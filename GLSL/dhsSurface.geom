#version 440 compatibility

layout (triangles) in;
layout (triangle_strip, max_vertices = 11) out;

uniform float diffuseRoughness;
uniform mat4 mvMatrix;
uniform float coatSpecularWeight;
uniform float coatSpecularRoughness;
uniform float diffuseWeight;

in VS_OUT 
{
	vec3 pos;
    vec3 normal;
	vec2 textureCoords0;
	mat3 ctbnMatrix;
} gs_in[];

out GS_OUT
{
	vec3 pos;
	vec3 normal;
	vec2 textureCoords0;
	vec3 tangent;
	vec3 bitangent;
	mat3 tbnMatrix;
	mat3 ctbnMatrix;
}gs_out; 


///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////

highp float rand(vec2 co)
{
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43758.5453;
    highp float dt= dot(co.xy ,vec2(a,b));
    highp float sn= mod(dt,3.14);
    return fract(sin(sn) * c);
}

vec3 getNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
} 

void GenerateCube(int index, vec4 position, float MAGNITUDE)
{
	gl_Position = position + vec4(-MAGNITUDE, -MAGNITUDE, MAGNITUDE, 0);
	gs_out.pos = vec3(gl_Position);
	gs_out.normal = getNormal();
	gs_out.textureCoords0 = gs_in[index].textureCoords0;
	EmitVertex();
	
	gl_Position = position + vec4(MAGNITUDE, -MAGNITUDE, MAGNITUDE, 0);
	gs_out.pos = vec3(gl_Position);
	gs_out.normal = getNormal();
	gs_out.textureCoords0 = gs_in[index].textureCoords0;
	EmitVertex();
	
	gl_Position = position + vec4(-MAGNITUDE, MAGNITUDE, MAGNITUDE, 0);
	gs_out.pos = vec3(gl_Position);
	gs_out.normal = getNormal();
	gs_out.textureCoords0 = gs_in[index].textureCoords0;
	EmitVertex();
	
	gl_Position = position + vec4(MAGNITUDE, MAGNITUDE, MAGNITUDE, 0);
	gs_out.pos = vec3(gl_Position);
	gs_out.normal = getNormal();
	gs_out.textureCoords0 = gs_in[index].textureCoords0;
	EmitVertex();
	
	gl_Position = position + vec4(-MAGNITUDE, MAGNITUDE, -MAGNITUDE, 0);
	gs_out.pos = vec3(gl_Position);
	gs_out.normal = getNormal();
	gs_out.textureCoords0 = gs_in[index].textureCoords0;
	EmitVertex();
	
	gl_Position = position + vec4(MAGNITUDE, MAGNITUDE, -MAGNITUDE, 0);
	gs_out.pos = vec3(gl_Position);
	gs_out.normal = getNormal();
	gs_out.textureCoords0 = gs_in[index].textureCoords0;
	EmitVertex();
	
	gl_Position = position + vec4(-MAGNITUDE, -MAGNITUDE, -MAGNITUDE, 0);
	gs_out.pos = vec3(gl_Position);
	gs_out.normal = getNormal();
	gs_out.textureCoords0 = gs_in[index].textureCoords0;
	EmitVertex();
	
	gl_Position = position + vec4(MAGNITUDE, -MAGNITUDE, -MAGNITUDE, 0);
	gs_out.pos = vec3(gl_Position);
	gs_out.normal = getNormal();
	gs_out.textureCoords0 = gs_in[index].textureCoords0;
	EmitVertex();
	EndPrimitive();	
}

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0*diffuseRoughness;
    vec3 direction = normal * magnitude; 
    return position + vec4(direction, 0.0);
} 

vec3 explode(vec3 position, vec3 normal)
{
    float magnitude = 2.0*diffuseRoughness;
    vec3 direction = normal * magnitude; 
    return position + vec3(direction);
} 

vec3 tangent()
{
	vec3 edge1 = vec3(gl_in[1].gl_Position) - vec3(gl_in[0].gl_Position);
	vec3 edge2 = vec3(gl_in[2].gl_Position) - vec3(gl_in[0].gl_Position);
	vec2 deltaUV1 = gs_in[1].textureCoords0 - gs_in[0].textureCoords0;
	vec2 deltaUV2 = gs_in[2].textureCoords0 - gs_in[0].textureCoords0;
	
	float f = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
	
	vec3 tangent1 = vec3(0.0);
	
	tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent1 = normalize(tangent1);
	
	return tangent1;	
}

vec3 bitangent()
{
	vec3 edge1 = vec3(gl_in[1].gl_Position) - vec3(gl_in[0].gl_Position);
	vec3 edge2 = vec3(gl_in[2].gl_Position) - vec3(gl_in[0].gl_Position);
	vec2 deltaUV1 = gs_in[1].textureCoords0 - gs_in[0].textureCoords0;
	vec2 deltaUV2 = gs_in[2].textureCoords0 - gs_in[0].textureCoords0;
	
	float f = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
	
	vec3 bitangent1 = vec3(0.0);
	
	bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent1 = normalize(bitangent1);  
	
	return bitangent1;	
}

///////////////////////////////////////////////////////////////////////////////////

void main()
{
    for(int i=0; i<3; i++)
	{
		vec3 shift = vec3(getNormal().x, getNormal().y, getNormal().z) * 0.1;
		
		gl_Position = explode(vec4(gl_in[i].gl_Position.x, gl_in[i].gl_Position.y, gl_in[i].gl_Position.z, gl_in[i].gl_Position.a), shift);
		gs_out.pos = explode(vec3(gs_in[i].pos.x, gs_in[i].pos.y, gs_in[i].pos.z), shift);
		gs_out.normal = gs_in[i].normal + shift*2*diffuseRoughness;
		gs_out.textureCoords0 = gs_in[i].textureCoords0;
		gs_out.tangent = tangent();
		gs_out.bitangent = bitangent();
		
		vec3 T = normalize(tangent());
		vec3 B = normalize(bitangent());
		vec3 N = normalize(gs_out.normal);
		gs_out.tbnMatrix = mat3(T, B, N);

		gs_out.ctbnMatrix = gs_in[0].ctbnMatrix;

		EmitVertex();	
	}
	EndPrimitive();
	
	//GenerateCube(0, gl_Position, 0.005);
}
