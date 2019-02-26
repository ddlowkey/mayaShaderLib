#version 450

layout (location = 0) in vec3 VertexPosition;

uniform mat4 projMatrix;
uniform mat4 mvMatrix;
uniform mat3 normalMatrix;

out vec3 TexCoords;

void main()
{
	TexCoords = VertexPosition;
	gl_Position = projMatrix * mvMatrix * vec4(VertexPosition, 1.0);
}