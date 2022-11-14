#version 410

in vec3 fragNormal;
in vec3 worldPos;

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
	// Color based on normals.
	//vec3 normalMeshColor = fragNormal * 0.5 + 0.5;

	// Lighting.
	vec3 normal = normalize(fragNormal);
	float nDotL = max(0, dot(normal, dirLightDir));

	// Surface lighting.
	vec3 irradiance = ambientColor + dirLightColor * nDotL;

	// Add pointlight lighting.
	vec3 toPointLight = pointLightPos - worldPos;
	float pointFalloff = (1.0 / dot(toPointLight, toPointLight));
	irradiance += pointLightColor * pointFalloff * pointLightIntensity;

	// Surface reflection.
	vec3 linearColor = meshColor * irradiance;

	// Color based on normals, lighting.
	outColor = vec4(pow(linearColor, vec3(1.0 / 2.2)), 1.0);
}