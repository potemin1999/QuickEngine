#version 440

#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack: enable
#extension GL_ARB_separate_shader_objects : enable

precision highp float;

layout(location = 0) out vec4 o_AlbedoSpec;
layout(location = 1) out vec4 o_Normal;
layout(location = 2) out vec4 o_Position;

layout(location = 0) uniform sampler2D u_AmbientTex;
layout(location = 1) uniform sampler2D u_NormalTex;

uniform float u_Brightness;
uniform float u_ZFar;

in vec2 v_UV;
in vec4 v_Normal;
in vec4 v_Position;
in vec4 v_ViewPosition;
in float v_Depth;

void main(){
    o_AlbedoSpec = vec4(u_Brightness,u_Brightness,u_Brightness,u_Brightness)*texture(u_AmbientTex,v_UV);
    o_Normal = vec4(normalize(v_Normal.xyz),1.0);
	float depth =  v_Position.z/u_ZFar;
    o_Position = (vec4(0.5)+0.5*v_Position);
    o_Position.z = depth;
	gl_FragDepth = depth;//*depth;
}
