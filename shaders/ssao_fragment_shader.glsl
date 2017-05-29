#version 330

precision highp float;

#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack: enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out float o_Occlusion;

layout(location = 0) uniform sampler2D u_DepthTex;
layout(location = 1) uniform sampler2D u_NormalTex;
layout(location = 2) uniform sampler2D u_PositionTex;
layout(location = 3) uniform sampler2D u_LightingTex;


uniform vec2 u_PixelSize;

uniform vec3 u_RandomMap[128];
uniform int u_Samples;
uniform float u_ZFar;
uniform float u_Aspect;
uniform mat4 u_Projection;

const float power = 0.5;
const float radius = 0.5;
const float bias = 0.075;
const vec2 noiseScale = vec2(1280.0/4.0, 720.0/4.0);

in vec2 v_UV;
in vec4 v_Normal;
in vec4 v_Position;
in float v_Depth;
in vec3 v_ViewRay;
in vec2 v_Scale;



float get_depth(in sampler2D s_depth,vec2 uv){
    return texture2D(s_depth,uv).x;
}








float get_occlusion(in sampler2D s_depth,in sampler2D s_position,in sampler2D s_normal,in sampler2D s_diffuse,vec2 uv){

	float depth = texture2D(s_depth,uv).x;
	if (depth==1.0) discard;

	vec3 pos = v_ViewRay*depth*u_ZFar;
	//vec3 pos = texture(s_position, uv).xyz; pos.x = -2.0*pos.x+1.0; pos.y = (-2.0*pos.y+1.0)/u_Aspect; pos.z=depth*u_ZFar;//pos.z*u_ZFar;
	//pos.y = pos_b.y;
	//return 0.5*pos.z+0.5;

	//pos = texture(s_position, uv).xyz;

	vec3 normal = texture(s_normal, uv).xyz*2.0-1.0;
	int p = int((uv.x*97+normal.x*39+depth*72+pos.y*76)/u_PixelSize[0])%(u_Samples/2)
	       +int((uv.y*43+normal.y*13+depth*78+pos.x*5)/u_PixelSize[1])%(u_Samples/2);
	vec3 rvec = u_RandomMap[p]*2.0-1.0;
	vec3 tangent = normalize(rvec-normal*dot(rvec, normal));
	vec3 bitangent = cross(normal,tangent);
	mat3 rotate = mat3(tangent, bitangent, normal);
	float acc = 0.0;

	for (int i=0; i<u_Samples; i++) {
		vec3 sample = rotate*u_RandomMap[i];
		//vec3 sample = u_RandomMap[i];
		sample = sample*radius+pos;
		vec4 shift = u_Projection*vec4(sample, 1.0);
		shift.xy /= shift.w;
		shift.xy = shift.xy*0.5+0.5;

		float sampleDepth = texture(s_depth, shift.xy).x*u_ZFar;
		//float dz = abs(depth*u_ZFar-sampleDepth);
		//float dx = abs(sample.x - pos.x);
		//float dy = abs(sample.y - pos.y);
		float rangeCheck = smoothstep(0.0, 1.0, radius/abs(pos.z-sampleDepth));//pow( dx*dx+dy*dy+dz*dz , 0.5 ));
		acc += step(sampleDepth, sample.z)*rangeCheck;
	}

	return pow(1.0-(acc/float(u_Samples)),power);
	//return 1.0-pow(acc/float(u_Samples),2.0);
}












void main(){
    //vec4 depth = texture2D(u_DepthTex,v_UV);
    //vec4 depth_color = vec4(depth.x,depth.x,depth.x,1.0);


    float occlusion = get_occlusion(u_DepthTex,u_PositionTex,u_NormalTex,u_LightingTex,v_UV*v_Scale);
	o_Occlusion = occlusion;
    //o_Occlusion = depth_color;//+0.005*texture2D(u_NormalTex,v_UV)+0.005*texture2D(u_PositionTex,v_UV);
}




















/*


float get_occlusion(in sampler2D s_depth,in sampler2D s_normal,in sampler2D s_position,vec2 uv){

	float depth = texture2D(s_depth,uv).x;
	if (depth==1.0) discard;
	vec3 pos = v_ViewRay*depth*u_ZFar;
	vec3 normal = vec3(0,0,1)*2.0-1.0;// texture(s_normal, uv).xyz*2.0-1.0;
	int p = int((uv.x*98+normal.x*28+depth*8972+pos.y*4076)/u_PixelSize[0])%(u_Samples/2)
	       +int((uv.y*453+normal.y*213+depth*44+pos.x*51)/u_PixelSize[1])%(u_Samples/2);
	vec3 rvec = u_RandomMap[p]*2.0-1.0;
	vec3 tangent = normalize(rvec-normal*dot(rvec, normal));
	vec3 bitangent = cross(normal,tangent);
	mat3 rotate = mat3(tangent, bitangent, normal);
	float acc = 0.0;

	for (int i=0; i<u_Samples; i++) {
		vec3 sample = rotate*u_RandomMap[i];
		sample = sample*radius+pos;
		vec4 shift = u_Projection*vec4(sample, 1.0);
		shift.xy /= shift.w;
		shift.xy = shift.xy*0.5+0.5;

		float sampleDepth = texture(s_depth, shift.xy).x*u_ZFar;
		//float sampleDepth = texture(s_position, shift.xy).z*u_ZFar;

		float rangeCheck = smoothstep(0.0, 1.0, radius/(abs(pos.z-sampleDepth))  );
		//acc+=rangeCheck;
		//acc += (sampleDepth >= sample.z + bias ? 1.0 : 0.0)*rangeCheck;
		acc += step(sampleDepth, sample.z)*rangeCheck;
	}

	return 1.0-pow(acc/float(u_Samples)),4.0);
}


*/













/*

float get_occlusion(in sampler2D s_depth,in sampler2D s_normal,in sampler2D s_position,vec2 uv){
	float depth = texture2D(s_depth,uv).x;
	if (depth==1.0) discard;
	//depth *= u_ZFar;

	vec3 pos = v_ViewRay*depth;
	//vec3 position = texture2D(s_position,uv).xyz;

	//vec3 pos = texture2D(s_position,uv).xyz;// pos*=u_ZFar;
	vec3 normal = texture(s_normal, uv).xyz;
	int p = int((uv.x*98+normal.x*28+depth*8972+pos.y*4076)/u_PixelSize[0])%(u_Samples/2)
	       +int((uv.y*453+normal.y*213+depth*44+pos.x*51)/u_PixelSize[1])%(u_Samples/2);

	vec3 rvec = u_RandomMap[p];
	vec3 tangent = normalize(rvec-normal*dot(rvec, normal));
	vec3 bitangent = cross(normal,tangent);
	mat3 rotate = mat3(tangent, bitangent, normal);

	float acc = 0.0;

	for (int i=0; i<u_Samples; i++) {
		vec3 sample = rotate*u_RandomMap[i];
		sample = sample*radius+pos;
		vec4 shift = u_Projection*vec4(sample, 1.0);
		shift.xy /= shift.w;
		shift.xy = shift.xy*0.5+0.5;

		float sampleDepth = texture(s_depth, shift.xy).x;//*u_ZFar;
		//float sampleDepth = texture(s_position, shift.xy).z;

		float rangeCheck = smoothstep(0.0, 1.0, radius/(abs(depth-sampleDepth))  );

		//acc += (sampleDepth >= shift.z + bias ? 1.0 : 0.0)*rangeCheck;
		acc += step(sampleDepth, shift.z)*rangeCheck;
	}

	return pow(1.0-(acc/float(u_Samples)), power);
	//return vec4(1.0 - (acc / float( u_Samples) ) );
	//return vec4(rvec,1.0);
}

*/
