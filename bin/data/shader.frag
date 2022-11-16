#version 410

in vec3 fragNormal;
in vec3 worldPos;
in vec2 fragUV;

uniform sampler2D colorTexture;
uniform sampler2D metallicTexture;
uniform sampler2D normalTexture;

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
	vec3 normal = normalize(fragNormal);
	float nDotL = max(0, dot(normal, dirLightDir));

	// Surface lighting.
	vec3 irradiance = ambientColor + dirLightColor * nDotL;


	//Spotlight lighting
	vec3 toSpotLight = spotLightPos - worldPos;
	vec3 spotLightDir = normalize(toSpotLight);
	float cosAngle = dot(spotLightDir, -spotLightConeDir);
	float spotFalloff = (1.0 / dot(toSpotLight, toSpotLight));
	float spotNDotL = max(0, dot(normal, spotLightDir));
	irradiance += spotLightColor * spotNDotL * spotFalloff * spotLightIntensity;
	

	// Add pointlight lighting.
	vec3 toPointLight = pointLightPos - worldPos;
	float pointFalloff = (1.0 / dot(toPointLight, toPointLight));
	irradiance += pointLightColor * pointFalloff * pointLightIntensity;

	// Surface reflection.
	vec3 linearColor = texture(colorTexture, fragUV) * irradiance;

	// Color based on normals, lighting.
	outColor = vec4(pow(linearColor, vec3(1.0 / 2.2)), 1.0);
}