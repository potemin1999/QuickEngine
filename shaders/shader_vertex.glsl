#version 330

#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec3 a_norm;

uniform mat4 u_MVP;
//uniform mat4 u_P;
uniform mat4 u_View;
uniform vec3 u_CamPos;

out vec4 v_Position;
out vec4 v_ViewPosition;
out vec2 v_UV;
out vec4 v_Normal;
out float v_Depth;

void main(){
    v_Position = u_MVP * vec4(a_pos,1.0) ;
    gl_Position = v_Position;
	//v_ViewPosition = v_Position;//u_V*u_M*vec4(a_pos,1.0);
	//u_V*vec4(a_pos,1.0) ;//v_Position;
    v_UV = a_uv;
    vec4 norm = u_View*vec4(a_norm,0.0);
    //vec4 norm = u_Look*vec4(a_norm,1.0);
	v_Normal = vec4(normalize( norm.xyz),1.0);
}
