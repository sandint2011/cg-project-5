#version 410

in mat3 TBN;
in vec3 fragTangent;
in vec3 worldPos;
in vec2 fragUV;

uniform sampler2D colorTexture;
uniform sampler2D metallicTexture;
uniform sampler2D normalTexture;

uniform samplerCube skyMap;
uniform samplerCube envMap;

uniform vec3 cameraPosition;

uniform mat3 normalMatrix;

// LitDrawNode uniforms.
uniform vec3 meshColor;

uniform vec3 ambientColor;

uniform vec3 dirLightColor;
uniform vec3 dirLightDir;

uniform vec3 spotLightColor;
uniform vec3 spotLightConeDir;
uniform vec3 spotLightPos;
uniform float spotLightCutoff;
uniform float spotLightIntensity;

uniform vec3 pointLightColor;
uniform vec3 pointLightPos;
uniform float pointLightCutoff;
uniform float pointLightIntensity;

out vec4 outColor;

void main()
{
	// Lighting.
	//vec3 normal = normalize(fragNormal);

	vec3 tsNormal = texture(normalTexture, fragUV).xyz * 2 - 1;
	vec3 wsNormal = normalize(TBN * tsNormal);

	vec3 tangent = fragTangent;
	vec3 bitangent = normalize(normalMatrix * cross(tangent, wsNormal));
	float nDotL = max(0, dot(wsNormal, dirLightDir));

	// Surface lighting.
	vec3 irradiance = ambientColor + dirLightColor * nDotL;

	// Skybox lighting.
	irradiance += texture(envMap, worldPos).xyz / 5; // 5 because otherwise it's way to bright.

	// Spotlight lighting
	vec3 toSpotLight = spotLightPos - worldPos;
	vec3 spotLightDir = normalize(toSpotLight);
	float cosAngle = dot(spotLightDir, -spotLightConeDir);
	float spotFalloff = (1.0 / dot(toSpotLight, toSpotLight));
	float spotNDotL = max(0, dot(wsNormal, spotLightDir));
	irradiance += spotLightColor * spotNDotL * spotFalloff * spotLightIntensity;

	// Add pointlight lighting.
	vec3 toPointLight = pointLightPos - worldPos;
	float pointFalloff = (1.0 / dot(toPointLight, toPointLight));
	irradiance += pointLightColor * pointFalloff * pointLightIntensity;

	// Surface reflection.
	vec3 linearColor = pow(texture(colorTexture, fragUV).rgb * irradiance, vec3(2.2));
	vec3 metallicColor = texture(metallicTexture, fragUV).rgb;
	metallicColor = vec3(metallicColor.r, metallicColor.r, metallicColor.r); // MetallicColor is red/black for some reason instead of white/black, so this is a hack to fix it.
	metallicColor = pow(metallicColor * irradiance, vec3(2.2));

	// Camera stuff.
	vec3 view = cameraPosition - worldPos;
	vec3 envLightDir = reflect(-view, wsNormal);
	vec3 envReflection = texture(skyMap, envLightDir).rgb / vec3(2); // Gamma correcting this causes it to disappear for some reason.

	// Color based on normals, lighting, etc.
	vec4 originalOutColor = vec4(pow(linearColor, vec3(1.0 / 2.2)), 1.0);

	vec3 shiny = envReflection * originalOutColor.rgb;
	vec3 notShiny = originalOutColor.rgb;

	// Just be entirely shiny (since metallic map causes shiny parts to be either blueish or solid white).
	outColor = vec4(shiny, 1.0);

	// Mix between full reflection and metal-only reflection (makes reflection look blueish).
	//outColor = vec4(mix(notShiny, shiny, normalize(metallicColor)), 1.0); 

	// How the video does it, but it doesn't work for some reason.
	//outColor = vec4(pow(metallicColor * envReflection, vec3(1.0 / 2.2)), 1.0);
}