#version 410

uniform samplerCube cubemap;

in vec3 fragPosition;

out vec4 outColor;

void main()
{
	outColor = texture(cubemap, fragPosition);
}