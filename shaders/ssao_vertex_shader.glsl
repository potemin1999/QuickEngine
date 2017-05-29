#version 330 core

#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require

layout(location = 0) in vec3 a_pos;

uniform float u_Fov;
uniform float u_Aspect;
uniform vec2 u_Scale;

out vec2 v_UV;
out vec3 v_ViewRay;
out vec2 v_Scale;

//const float fov=0.7;
//const float aspect=1.6;

void main(){
    gl_Position = vec4(a_pos,1.0);
	v_Scale = u_Scale;
    v_UV = vec2(0.5*(a_pos.x+1),0.5*(a_pos.y+1));
  	v_ViewRay = vec3(-a_pos.x*u_Aspect*tan(u_Fov/2), -a_pos.y*tan(u_Fov/2), 1.0);
}