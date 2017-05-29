#version 330 core

#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require

layout(location = 0) in vec3 a_pos;


out vec2 v_UV;

void main(){
    gl_Position = vec4(a_pos,1.0);
    v_UV = vec2(0.5*(a_pos.x+1),0.5*(a_pos.y+1));
}