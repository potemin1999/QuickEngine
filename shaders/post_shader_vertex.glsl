#version 330 core

#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require

layout(location = 0) in vec3 a_pos;



out vec2 uv;
//out vec2 pixelSize;
//out vec2 screenSize;

void main(){
  gl_Position = vec4(a_pos,1.0);
  uv = vec2(0.5*(a_pos.x+1),0.5*(a_pos.y+1));
 // pixelSize = u_PixelSize;
  //screenSize = u_ScreenSize;
}