#version 440 compatibility

layout(location=0) in vec3 vPos;
layout(location=1) in vec2 vTexCoords;
layout(location=2) in vec3 vNormal;
layout(location=3) in vec3 vTangent;
layout(location=4) in vec3 vBitangent;

uniform mat4 projMatrix;
uniform mat4 mvMatrix;
uniform mat3 normalMatrix;


out VS_OUT {
	vec3 pos;
    vec3 normal;
	vec2 textureCoords0;
	mat3 ctbnMatrix;
} vs_out;

void main()
{
	vs_out.pos = vec3(mvMatrix * vec4(vPos.xyz, 1.0));
	vs_out.normal = normalMatrix * vNormal;
	vs_out.textureCoords0 = vTexCoords.xy;
	
    gl_Position = projMatrix * mvMatrix * vec4(vPos.xyz, 1.0);
}
