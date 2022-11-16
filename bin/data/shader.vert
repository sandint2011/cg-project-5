#version 410

layout (location = 0) in vec3 position;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 uv;

// LitDrawNode uniforms.
uniform mat4 mvp; // Movel-view-projection transform.
uniform mat4 model;
uniform mat3 normalMatrix;

out vec3 fragNormal;
out vec3 worldPos;
out vec2 fragUV;

void main()
{
	gl_Position = mvp * vec4(position, 1.0);
	fragNormal = normalMatrix * normal;
	fragUV = vec2(uv.x, 1.0 - uv.y);

	// For spotlight.
	worldPos = (model * vec4(position, 1.0)).xyz;
}