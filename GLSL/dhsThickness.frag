#version 440 compatibility

in vec3 normal, pos;
in vec2 textureCoords0;

uniform float depthScale;
uniform float transmissionDepth;
uniform vec2 Size;
float near = 0.1; 
float far  = 15.0;
  
  
void main()
{
    float depth = depthScale * gl_FragCoord.z;
	gl_FragColor = vec4(depth , 0, 0, 0);
}
