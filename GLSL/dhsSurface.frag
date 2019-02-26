#version 440 compatibility

//UI parameter
uniform float diffuseWeight;
uniform float specularWeight;

uniform float diffuseRoughness;
uniform float specularRoughness;

uniform float metalness;
uniform float specularIOR;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 transmissionColor;
uniform vec3 transmissionScatterColor;
uniform vec3 emissionColor;
uniform vec3 coatSpecularColor;
uniform float coatSpecularWeight;
uniform float coatSpecularRoughness;
uniform float transmissionDepth;
uniform float transmissionWeight;
uniform float subsurfaceWeight;
uniform vec3 subsurfaceColor;
uniform vec3 subsurfaceRadiusColor;
uniform float subsurfaceScale;
uniform samplerCube cube_texture;
uniform mat3 normalMatrix;

uniform float light1_intensity;
uniform float light1_positionx;
uniform float light1_positiony;
uniform float light1_positionz;

uniform int useAlbedoMap;
uniform sampler2D textureAlbedoMap;
uniform int useNormalMap;
uniform sampler2D textureNormalMap;
uniform int useRoughnessMap;
uniform sampler2D textureRoughnessMap;
uniform int useSpecularMap;
uniform sampler2D textureSpecularMap;
uniform int useSubsurfaceRadiusMap;
uniform sampler2D textureSubSurfaceRadiusMap;
uniform int useMetalnessMap;
uniform sampler2D textureMetalnessMap;
uniform int useEmissiveMap;
uniform sampler2D textureEmissiveMap;
uniform int useDisplacementMap;
uniform sampler2D textureDisplacementMap;


uniform sampler2D thicknessMap;
uniform int depthScale;
uniform vec2 Size;

in GS_OUT
{
	vec3 pos;
	vec3 normal;
	vec2 textureCoords0;
	vec3 tangent;
	vec3 bitangent;
	mat3 tbnMatrix;
	mat3 ctbnMatrix;
}fs_in; 

//in vec3 normal, pos;
//in vec2 textureCoords0;

#define PI 3.1415926
#define E 2.71828

///////////////////////////////////////////////////////////////////////////////////

vec3 pos = fs_in.pos;
vec2 textureCoords0 = vec2(fs_in.textureCoords0.x, -fs_in.textureCoords0.y);

vec3 normalMap = vec3(texture(textureNormalMap, textureCoords0));
vec3 normal = fs_in.normal * (1 - useNormalMap) + normalize(fs_in.tbnMatrix * normalize(normalMap * 2.0 - 1.0)) * useNormalMap;


vec3 diffuseMap = diffuseColor * (1 - useAlbedoMap) + vec3(texture(textureAlbedoMap, textureCoords0)) *  useAlbedoMap;

float roughnessMap = specularRoughness * (1 - useRoughnessMap) + float(texture(textureRoughnessMap, textureCoords0)) * specularRoughness * useRoughnessMap;

vec3 subsurfaceRadiusColorMap = subsurfaceRadiusColor * (1 - useSubsurfaceRadiusMap) + vec3(texture(textureSubSurfaceRadiusMap, textureCoords0)) * useSubsurfaceRadiusMap;
///////////////////////////////////////////////////////////////////////////////////

//shader parameter

//base
//float diffuseWeight = 1.0;
//vec3 diffuseMap = vec3(texture(textureAlbedoMap, textureCoords0)); 
//float metalness = 0.0;

//specular
int distributeFunction = 0;
//float specularRoughness = 0.5;
//float specularWeight = 1.0;

float reflectWeight = 0.0;
//vec3 specularColor = vec3(1, 1, 1);

float shininess = 1.0;	
float ior = 1.5;

//float f0 = 0.04 + 0.96 * metalness;
float fresnelScale = 1.0;
float fresnelPower = 3.0;
//vec3 f0 = vec3(0.04);
vec3 f0 = mix(vec3(0.04), diffuseMap.xyz * 0.99, metalness);


//transmission
//float transmissionWeight = 0.0;
vec3 refractColor = vec3(1.0, 1.0, 1.0);



///////////////////////////////////////////////////////////////////////////////////

//normalize

vec3 lightPos = vec3((light1_positionx - 0.5) * 10, (light1_positiony - 0.5) * 10, (light1_positionz - 0.5) * 10);
vec3 nlight = normalize(lightPos - pos);	
vec3 neye  = normalize(-pos);
vec3 nnormal = normalize(normal);
vec3 nhalf = normalize(neye + nlight);

float NdL = max(dot(nnormal, nlight), 0);
float NdV = max(dot(nnormal, neye), 0);
float NdH = max(dot(nnormal, nhalf), 0);
float HdV = max(dot(nhalf, neye), 0);
float LdV = max(dot(nlight, neye), 0);

//vector
vec3 vreflect = reflect(-neye, nnormal);
vec3 vrefract = refract(-neye, nnormal, 1.0 / specularIOR);

vec3 vrefractR = refract(-neye, nnormal, 1.0 / specularIOR + 0.0338);
vec3 vrefractG = refract(-neye, nnormal, 1.0 / specularIOR + 0.0254);
vec3 vrefractB = refract(-neye, nnormal, 1.0 / specularIOR + 0.0234);

//attenuation
float lightAttenuation = 20.0 / dot(lightPos - pos, lightPos - pos);

///////////////////////////////////////////////////////////////////////////////////

const float EPSILON_MU = 0.0;
const vec3 one = vec3 (1.0);

float sigma_a = 0.3;
float sigma_s = 0.8;
float g = 0.9;
float n = 1.3;
float C1 = -9.233772 + 22.2272*n - 20.9292*pow(n,2) + 10.2291*pow(n,3) - 2.54396*pow(n,4) + 0.254913*pow(n,5);
float C2 = -1641.1 + 135.926 / pow(n,3) - 656.175/pow(n,2) + 1376.53/n + 1213.67*n - 568.556*pow(n,2) + 164.798*pow(n,3) - 27.0181*pow(n,4) + 1.91826*pow(n,5);
float C_s = (1 - C1) / 4;
float C_e = (1 - C2) / 2;

float D = 1 / (3 * (sigma_a + sigma_s * (1-g)));
float transmission = sqrt(sigma_a / D);
float de = (2.131 * D) / sqrt((sigma_s * (1-g))/(sigma_a + sigma_s * (1-g)));
vec3 one_over_three_ext = one/pow((3 * (sigma_a + sigma_s)), 2);
float A = (1 - C_e) / (2 * C_s);

///////////////////////////////////////////////////////////////////////////////////

//mix function

vec3 lerp(vec3 a, vec3 b, float s)
{
    return vec3(a+(b-a)*s);       
}

float lerp(float a, float b, float s)
{
    return float(a+(b-a)*s);       
}



//distribute function

float D_blinn(in float roughness, in float NdH)
{
    float m = roughness * roughness;
    float m2 = m * m;
    float n = 2.0 / m2 - 2.0;
    return (n + 2.0) / (2.0 * PI) * pow(NdH, n);
}

float D_beckmann(in float roughness, in float NdH)
{
    float m = roughness * roughness;
    float m2 = m * m;
    float NdH2 = NdH * NdH;
    return exp((NdH2 - 1.0) / (m2 * NdH2)) / (PI * m2 * NdH2 * NdH2);
}

float D_GGX(float roughness,float NdH)
{
    float m = pow(roughness, 2);
    float m2 = m * m;
    float d = (NdH * m2 - NdH) * NdH + 1.0;
    return m2 / (PI * d * d);
}

float DistributionGGX(vec3 N, vec3 H, float a)
{
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float nom    = a2;
    float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
    denom        = PI * denom * denom;
	
    return nom / denom;
}

float G_schlick(in float roughness, in float NdV, in float NdL)
{
    float k = roughness * roughness * 0.5;
    float V = NdV * (1.0 - k) + k;
    float L = NdL * (1.0 - k) + k;
    return 0.25 / (V * L);
}

float GeometrySchlickGGX(float NdV, float k)
{
    float nom   = NdV;
    float denom = NdV * (1.0 - k) + k + 0.01;
	
    return nom / denom;
}
  
float GeometrySmith(float k, float NdV, float NdL)
{

    float ggx1 = GeometrySchlickGGX(NdV, k);
    float ggx2 = GeometrySchlickGGX(NdL, k);
	
    return ggx1 * ggx2;
}

vec3 cooktorrance_specular(float NdL, float NdV, float NdH, vec3 specular, float roughness)
{
	float D = D_GGX(roughness, NdH);

	if(distributeFunction == 1)
	{
		D = D_blinn(roughness, NdH);
	}

	if(distributeFunction == 2)
	{
		D = D_beckmann(roughness, NdH);
	}

	if(distributeFunction == 3)
	{
		D = D_GGX(roughness, NdH);
	}

    float G = GeometrySmith(roughness, NdV, NdL);

    float rim = mix(1.0 - roughness * metalness * 0.0, 1.0, NdV);

    return (1.0 / rim) * specular * G * D;
}

vec3 fresnel_factor(in vec3 f0, in float product)
{
    return mix(f0, vec3(1.0), pow(1.01 - product, 5.0));
}

float phong_diffuse()
{
    return (1.0 / PI);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}


//intersection
float raySphereIntersect(vec3 direction, vec3 center) 
{
	float dist = 0;
	vec3 cp = center - pos;
	vec3 d = direction;
	dist = dot(cp, direction);


	return dist;
}

float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec3 S_infinite ( vec3 _r , vec3 _r_sq , float x_dot_w12 , float no_dot_w12 , float x_dot_no )
{
	//vec3 _r_sq = _r * _r;
	float D_rev = 1 / D;
	float three_D = 3 * D;
	
	vec3 _r_tr = transmission * _r ;
	vec3 _r_tr_p1 = _r_tr + one ;
	vec3 _T = exp (- _r_tr );
	vec3 coeff = _T / ( _r * _r_sq ) ;
	vec3 first = C_s * ( _r_sq * D_rev + 3 * _r_tr_p1 * x_dot_w12 );
	vec3 second = C_e * ( three_D * _r_tr_p1 * no_dot_w12 - ( _r_tr_p1 + three_D * (3 * _r_tr_p1 + _r_tr * _r_tr ) / ( _r_sq ) * x_dot_w12 ) * x_dot_no ) ;
	vec3 _S = coeff * ( first - second );
	return _S ;
}

vec3 S_infinite_vec ( vec3 _r , vec3 _r_sq , vec3 x_dot_w12 , float no_dot_w12 , vec3 x_dot_no )
{
	//vec3 _r_sq = _r * _r;
	float D_rev = 1 / D;
	float three_D = 3 * D;

	vec3 _r_tr = transmission * _r ;
	vec3 _r_tr_p1 = _r_tr + 1.0 ;
	vec3 _T = exp (- _r_tr );
	vec3 coeff = _T / ( _r * _r_sq ) ;
	vec3 first = C_s * ( _r_sq * D_rev + 3 * _r_tr_p1 * x_dot_w12 );
	vec3 second = C_e * ( three_D * _r_tr_p1 * no_dot_w12 - ( _r_tr_p1 + three_D * (3 * _r_tr_p1 + _r_tr * _r_tr ) / ( _r_sq ) * x_dot_w12 ) * x_dot_no ) ;
	vec3 _S = coeff * ( first - second );
	return _S ;
}

vec3 bssrdf ( vec3 _xi , vec3 _nin , vec3 _wi , vec3 _xo , vec3 _no )
{
	vec3 _x = _xo - _xi ;
	float r_sqr = dot (_x , _x ) ;
	vec3 _w12 = refract( _wi , _nin , 1/specularIOR);
	float mu = -dot ( _no , _w12 );
	float dot_x_w12 = dot (_x , _w12 );
	float dot_x_no = dot (_x , _no );
	vec3 _r_sqr = r_sqr.xxx ;
	vec3 _dr_sqr = _r_sqr ;
	float edge = step (0.0, mu); // == 1.0 if mu > EPSILON_MU
	float de_sqr = de * de;
	vec3 project = vec3 ( sqrt ( r_sqr - dot_x_w12 * dot_x_w12 )/ sqrt ( _r_sqr + de_sqr )) ;
	vec3 _D_prime = abs ( mu ) * D * edge + one_over_three_ext * (1.0 - edge );
	_dr_sqr += _D_prime * ( _D_prime + 2 * de * project * edge );
	vec3 _dr = sqrt ( _dr_sqr );
	float edge_nistar = step (abs( dot_x_no ) ,0.01); // 1.0 if dot_x_no > 0.01
	vec3 _t = normalize ( cross ( _nin , _x ));
	vec3 _nistar = _nin * edge_nistar + cross ( normalize ( _x ) ,_t ) * (1.0 - edge_nistar );
	mat3 _xov = mat3 ( _x , _x , _x ) - outerProduct ( _nistar , one * 2 * A * de ) ;
	vec3 _dv_sqr = vec3 ( dot ( _xov [0] , _xov [0]) ,dot( _xov [1] , _xov [1]) ,dot ( _xov[2] , _xov [2]) );
	vec3 _dv = sqrt ( _dv_sqr );
	vec3 _wv = _w12 - 2 * dot ( _w12 , _nistar ) * _nistar ;
	vec3 _x_dot_wv = _wv * _xov ;
	vec3 _x_dot_no = _no * _xov ;
	vec3 _S_r = S_infinite ( _dr , _dr_sqr , dot_x_w12 , -mu , dot_x_no );
	vec3 _S_v = S_infinite_vec ( _dv , _dv_sqr , _x_dot_wv , dot ( _no , _wv ) , _x_dot_no ) ;
	vec3 _S = _S_r - _S_v ;
	//_S *= fresnel_T ( _wi , _nin );
	_S = max ( vec3 (0.0) ,_S );
	
	return _S ;
	

}

///////////////////////////////////////////////////////////////////////////////////

void main()
{	


	
	///////////////////////////////////////////////////////////////////////////////////

	//fresnel		
	vec3 fresnel = mix(f0, vec3(1.0), fresnelScale * pow(1.01 - max(dot(neye, nnormal), 0.0), fresnelPower));
	float ffresnel = mix(0.04, 1.0, fresnelScale * pow(1.01 - max(dot(neye, nnormal), 0.0), fresnelPower));
	
	//transmission	
	//vec3 rawRefract = vec3(textureCubeLod(cube_texture, vrefract, roughnessMap * 11));
	
	vec3 rawRefractR = vec3(textureCubeLod(cube_texture, vrefractR, roughnessMap * 11));
	vec3 rawRefractG = vec3(textureCubeLod(cube_texture, vrefractG, roughnessMap * 11));
	vec3 rawRefractB = vec3(textureCubeLod(cube_texture, vrefractB, roughnessMap * 11));
	
	vec3 rawRefract = vec3(rawRefractR.r, rawRefractG.g, rawRefractB.b);
	

	//reflection	
	vec3 rawReflect = vec3(textureCubeLod(cube_texture, vreflect, 11 * roughnessMap));

	
	vec3 reflectBlur = vec3(textureCubeLod(cube_texture, vreflect, 11 * roughnessMap));
	vec3 reflectFresnel = mix(f0, reflectBlur, fresnel);
	vec3 reflection = rawReflect * reflectWeight;


	///////////////////////////////////////////////////////////////////////////////////	

	//specular	
	//float s = 0.0;
	
	//vec3 ref = normalize(reflect(nlight, nnormal));
	//s = pow(max(dot(neye, ref), 0.0), shininess);

	//vec3 specular = vec3(gl_LightSource[0].specular * s * vec4(specularColor,1.0)) * specularWeight;
	

	vec3 specular = mix(vec3(0.04), diffuseMap, metalness);
	vec3 envspec = vec3(textureCubeLod(cube_texture, vreflect, roughnessMap * 11));

	//cook-toorance BRDF
	vec3 specfresnel = fresnel_factor(f0, HdV);
    vec3 specref = cooktorrance_specular(NdL, NdV, NdH, specfresnel, roughnessMap);

	///////////////////////////////////////////////////////////////////////////////////

	//diffuse
	//float d = max(dot(nlight, normal), 0.0);	
	//vec3 diffuse = vec3(gl_LightSource[0].diffuse * d * vec4(diffuseMap, 1.0));
	
	
	mat3x3 tnrm = transpose(gl_NormalMatrix);
    vec3 envdiff = textureCubeLod(cube_texture, tnrm * nnormal, 10).xyz;

    vec3 diffref = (diffuseMap - specfresnel) * phong_diffuse() * NdL;

    ///////////////////////////////////////////////////////////////////////////////////	
	//compute irradiance
    vec3 irradiance = vec3(0.0);  

	vec3 up    = vec3(0.0, 1.0, 0.0);
	vec3 right = cross(up, normal);
	up         = cross(normal, right);

	float sampleDelta = 0.2;
	float nrSamples = 0.0; 
	for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
	{
	    for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
	    {
	        // spherical to cartesian (in tangent space)
	        vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
	        // tangent space to world
	        vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * nnormal; 

	        irradiance += texture(cube_texture, sampleVec).rgb * cos(theta) * sin(theta);
	        nrSamples++;
	    }
	}
	vec3 domeLight = mix(reflectFresnel, PI * irradiance * (1.0 / float(nrSamples)), 1);
	irradiance = mix(reflectFresnel, PI * irradiance * (1.0 / float(nrSamples)), roughnessMap);
	
	
	///////////////////////////////////////////////////////////////////////////////////
	//BSSRDF
	vec3 sss = vec3(0);
	vec3 xo = vec3(0);
	vec3 no = vec3(0);
	int loop = 1;
	for (int i = 0; i < loop; i++)  
	{  		
		xo = vec3(pos.x + rand(vec2(i*0.05, i*0.05)), pos.y + rand(vec2(-i*0.05, i*0.05)), pos.z + rand(vec2(i*0.05, -i*0.05)));
		no = normalize(vec3(nnormal.x + rand(vec2(i*0.05, i*0.05)), nnormal.y + rand(vec2(-i*0.05, i*0.05)), nnormal.z + rand(vec2(i*0.05, -i*0.05))));
		//sss += bssrdf(pos, nnormal, -nlight, xo, no);
	}
	sss += bssrdf(pos, nnormal, -nlight, xo, no);
	sss = sss *(subsurfaceScale/10)/ loop * subsurfaceColor * diffuseMap;
	///////////////////////////////////////////////////////////////////////////////////
	
	//absorption

	//blur
	vec2 fboTexCoord = gl_FragCoord.xy / Size;
	
	float dx = 0.2 * roughnessMap;
	
	vec2 a = vec2(-dx, dx);
	vec2 b = vec2( 0,  dx);
	vec2 c = vec2( dx, dx);
	vec2 d = vec2(-dx,  0);
	vec2 e = vec2( dx,  0);
	vec2 f = vec2(-dx,-dx);
	vec2 g = vec2( 0, -dx);
	vec2 h = vec2( dx,-dx);
		
	float allAbsorption = 0.0;
	float blur = 0.0;
	for(int i = 0; i< 30; i++)
	{
		float random = rand(vec2(i +0.01 , -i + 0.01));
		allAbsorption  = abs(texture(thicknessMap, fboTexCoord + a * random)).x;
		allAbsorption += abs(texture(thicknessMap, fboTexCoord + b * random)).x;
		allAbsorption += abs(texture(thicknessMap, fboTexCoord + c * random)).x;
		allAbsorption += abs(texture(thicknessMap, fboTexCoord + d * random)).x;
		allAbsorption += abs(texture(thicknessMap, fboTexCoord + e * random)).x;
		allAbsorption += abs(texture(thicknessMap, fboTexCoord + f * random)).x;
		allAbsorption += abs(texture(thicknessMap, fboTexCoord + g * random)).x;
		allAbsorption += abs(texture(thicknessMap, fboTexCoord + h * random)).x;
		allAbsorption *= 0.125;
		blur += allAbsorption;
	}
	blur = blur/30;
	
	float thickness = abs(texture2DLod(thicknessMap, fboTexCoord, roughnessMap * 11)).x;
    if (thickness < 0.0)
    {
        thickness = 0.0;
    }
	float sigma = 100 * transmissionDepth;
	float absorption = exp(-sigma*blur);
		
	///////////////////////////////////////////////////////////////////////////////////
	
	//compute light
	vec3 reflected_light = vec3(0);
	vec3 diffuse_light = vec3(0);
	vec3 IBL_light = vec3(0);
	
	//point light
	vec3 light1_color = vec3(1, 1, 1);
	
	vec3 kS = fresnelSchlickRoughness(NdV, f0, roughnessMap);
	vec3 kD = 1.0 - kS;
	vec3 IBL_diffuse = irradiance * diffuseMap;
	vec3 IBL_ambient = kD * IBL_diffuse;
	
	reflected_light += specref * light1_intensity;
	diffuse_light += diffref * light1_intensity * light1_color;
	diffuse_light += diffuseMap * domeLight * (1.0 - metalness);

	IBL_light += mix(vec3(0.04), reflectFresnel, metalness);

	vec3 allReflect = diffuse_light * diffuseWeight + reflected_light * specularWeight + IBL_light * fresnel;
	vec3 allRefract = mix(rawRefract * transmissionColor  * absorption + reflected_light * specularWeight, rawReflect, ffresnel * metalness);
	vec3 allTransmission = mix(diffuse_light * diffuseWeight * transmissionColor * absorption + reflected_light * specularWeight, rawReflect, ffresnel * metalness);
	float scatter = raySphereIntersect(nlight, vec3(0))*length(nlight);
	
	vec3 BSDF = mix(allReflect, allRefract, transmissionWeight);
	vec3 BSDFNoRefract = mix(allReflect, allTransmission, transmissionWeight);
	vec3 BSSRDF = mix(BSDF, BSDFNoRefract + sss * absorption * subsurfaceRadiusColorMap, subsurfaceWeight);

	//output	
	gl_FragColor = vec4(BSSRDF, 1);	
}
