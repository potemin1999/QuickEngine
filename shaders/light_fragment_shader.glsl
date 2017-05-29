#version 330

#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack: enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 o_Diffuse;

layout(location = 0) uniform sampler2D u_AlbedoSpecTex;
layout(location = 1) uniform sampler2D u_NormalTex;
layout(location = 2) uniform sampler2D u_PositionTex;


uniform vec2 u_PixelSize;

in vec2 v_UV;
in vec4 v_Normal;
in vec4 v_Position;
in float v_Depth;

void main(){
    o_Diffuse = vec4(1.0)-1.6*texture2D(u_AlbedoSpecTex,v_UV)+0.05*texture2D(u_PositionTex,v_UV)+0.1*texture2D(u_NormalTex,v_UV);
  //o_Diffuse = vec4(1.0);
}