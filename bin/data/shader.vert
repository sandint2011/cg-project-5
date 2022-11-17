#version 410

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 tangent;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 uv;

// LitDrawNode uniforms.
uniform mat4 mvp; // Movel-view-projection transform.
uniform mat4 model;
uniform mat3 normalMatrix;

out mat3 TBN;
out vec3 worldPos;
out vec2 fragUV;
out vec3 fragTangent;

void main()
{
	gl_Position = mvp * vec4(position, 1.0);

	vec3 T = normalize(normalMatrix * tangent.xyz);
	vec3 B = normalize(normalMatrix * cross(tangent.xyz, normal));
	vec3 N = normalize(normalMatrix * normal);


	TBN = mat3(T,B,N);

	fragTangent = tangent;
	fragUV = vec2(uv.x, 1.0 - uv.y);

	// For spotlight.
	worldPos = (model * vec4(position, 1.0)).xyz;
}