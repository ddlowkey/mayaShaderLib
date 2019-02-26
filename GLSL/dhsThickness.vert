#version 440 compatibility

layout(location=0) in vec3 vPos;
layout(location=1) in vec2 vTexCoords;
layout(location=2) in vec3 vNormal;

uniform mat4 projMatrix;
uniform mat4 mvMatrix;
uniform mat3 normalMatrix;

out vec3 normal, pos;
out vec2 textureCoords0;

void main()
{
	pos = vec3(mvMatrix * vec4(vPos.xyz, 1.0));

	
	normal = normalMatrix * vNormal;
	textureCoords0 = vTexCoords.xy;
    gl_Position = projMatrix * mvMatrix * vec4(vPos.xyz, 1.0);
}
