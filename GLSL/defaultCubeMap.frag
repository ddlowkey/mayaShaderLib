#version 450

in vec3 TexCoords;

uniform samplerCube cube_texture;

layout (location = 0) out vec4 FragColor;

void main()
{
	FragColor = vec4(vec3(texture(cube_texture, TexCoords)),1.0);
}