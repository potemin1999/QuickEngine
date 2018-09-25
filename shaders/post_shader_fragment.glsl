#version 440 core

#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack: enable

precision highp float;

//#define MAX_POINT_LIGHTS 256;

struct PointLight{
  vec3 position;
  float intense;
};

const int MAX_POINT_LIGHTS = 256;
uniform int point_lights_length;
uniform PointLight point_lights[MAX_POINT_LIGHTS];

uniform int u_SuperSampling;
uniform float u_Vignette;
uniform float u_Brightness;
uniform vec2 u_PixelSize;
uniform vec2 u_ScreenSize;
uniform int u_FXAA;
uniform int u_DrawingMode;
uniform float u_Gamma;

layout(location = 0) uniform sampler2D u_AlbedoSpecTex;
layout(location = 1) uniform sampler2D u_NormalTex;
layout(location = 2) uniform sampler2D u_PositionTex;
layout(location = 3) uniform sampler2D u_DiffuseTex;
layout(location = 4) uniform sampler2D u_OcclusionTex;

layout(location = 0) out vec4 gl_Color;

in vec2 uv;
//in vec2 pixelSize;

float mainsample_weight = 1.0;
float sample_weight = 0.5;
float sample_sum = 3;
bool multisampling = false;

vec4 color_fxaa(in sampler2D texture,vec2 uv,vec2 screen_size){
  vec4 color = vec4(1.0);
  float FXAA_SPAN_MAX = 8.0; //8.0
  float FXAA_REDUCE_MUL = 1.0/2.0; //8.0
  float FXAA_REDUCE_MIN = 1.0/32.0; //128.0
  vec3 rgbNW=texture2D(texture,uv+(vec2(-u_PixelSize.x,-u_PixelSize.y))).xyz;
  vec3 rgbNE=texture2D(texture,uv+(vec2( u_PixelSize.x,-u_PixelSize.y))).xyz;
  vec3 rgbSW=texture2D(texture,uv+(vec2(-u_PixelSize.x, u_PixelSize.y))).xyz;
  vec3 rgbSE=texture2D(texture,uv+(vec2( u_PixelSize.x, u_PixelSize.y))).xyz;
  vec3 rgbM=texture2D(texture,uv).xyz;
  vec3 luma=vec3(0.299, 0.587, 0.114);
  float lumaNW = dot(rgbNW, luma);
  float lumaNE = dot(rgbNE, luma);
  float lumaSW = dot(rgbSW, luma);
  float lumaSE = dot(rgbSE, luma);
  float lumaM  = dot(rgbM,  luma);
  float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
  float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
  vec2 dir;
  dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
  dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));
  float dirReduce = max( (lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);
  float rcpDirMin = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);
  dir = min(vec2( FXAA_SPAN_MAX,  FXAA_SPAN_MAX),
            max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),dir * rcpDirMin)) / screen_size;
  vec3 rgbA = (1.0/2.0) * (
      texture2D(texture, uv.xy + dir * (1.0/3.0 - 0.5)).xyz +
      texture2D(texture, uv.xy + dir * (2.0/3.0 - 0.5)).xyz);
  vec3 rgbB = rgbA * (1.0/2.0) + (1.0/4.0) * (
      texture2D(texture, uv.xy + dir * (0.0/3.0 - 0.5)).xyz +
      texture2D(texture, uv.xy + dir * (3.0/3.0 - 0.5)).xyz);
  float lumaB = dot(rgbB, luma);
  if((lumaB < lumaMin) || (lumaB > lumaMax)){
        color.xyz=rgbA;
  }else{
        color.xyz=rgbB;
  }
  return color;
}

vec4 color_msaa(in sampler2D texture,vec2 uv){
  //vec4 color = vec4(1.0);
  float sample_weight = 0.2;
  vec4 color = sample_weight*texture2D(texture,uv);
  color+= sample_weight*texture2D(texture, vec2(uv.x+u_PixelSize.x,uv.y             ));
  color+= sample_weight*texture2D(texture, vec2(uv.x            ,uv.y+u_PixelSize.x ));
  color+= sample_weight*texture2D(texture, vec2(uv.x-u_PixelSize.x,uv.y             ));
  color+= sample_weight*texture2D(texture, vec2(uv.x            ,uv.y-u_PixelSize.x ));
  //color/=2;
  return color;
}

vec4 color_simple(in sampler2D texture,vec2 uv){
  return texture2D(texture,uv);
}

vec4 get_color(in sampler2D texture,vec2 uv){
  if (u_FXAA==1){
    return color_fxaa(texture,uv,u_ScreenSize);
  }else if (multisampling){
    return color_msaa(texture,uv);
  }else{
    return color_simple(texture,uv);
  }
}

vec4 get_supersampled_color(in sampler2D texture,vec2 uv_ss){
  vec4 color_lt = get_color(texture,uv_ss+vec2(0          ,0));
  vec4 color_rt = get_color(texture,uv_ss+vec2(u_PixelSize.x,0));
  vec4 color_lb = get_color(texture,uv_ss+vec2(0          ,u_PixelSize.y));
  vec4 color_rb = get_color(texture,uv_ss+vec2(u_PixelSize.x,u_PixelSize.y));
  return (color_rb+color_lb+color_rt+color_lt)/4;
}

vec4 get_vignette_color(vec4 color,vec2 vignette_uv,float strength){
  float m1 = 2.0 - pow(2,            max(strength*(length(vignette_uv)-0.8),0)         );
  return color*m1;
}

vec4 draw_text(vec2 uv, vec2 col_str,vec2 screen_size){
  float col = floor((uv.x)*col_str.x);
  float str = floor((uv.y)*col_str.y);
  int symbol = 49;
  vec2 screen_symbol_size = vec2(screen_size.x/col_str.x,screen_size.y/col_str.y);
  vec2 screen_symbol_xy = vec2(uv.x+col*screen_symbol_size.x/screen_size.x,      (uv.y)+str*screen_symbol_size.y/screen_size.y);
  vec2 font_symbol_uv = vec2(  col_str.x*(uv.x-col/col_str.x)  ,   col_str.y*(uv.y-str/col_str.y)   );
  return vec4(font_symbol_uv.x/2,   screen_symbol_xy.x/2  ,font_symbol_uv.y,max(font_symbol_uv.x,font_symbol_uv.y) );
}

/*
float get_occlusion_p(vec2 uv){
    vec4 c1 = color_msaa(u_OcclusionTex,vec2(uv.x+0.6*u_PixelSize.x,uv.y                  ));
	vec4 c2 = color_msaa(u_OcclusionTex,vec2(uv.x                  ,uv.y+0.6*u_PixelSize.y));
	vec4 c3 = color_msaa(u_OcclusionTex,vec2(uv.x-0.6*u_PixelSize.x,uv.y                  ));
	vec4 c4 = color_msaa(u_OcclusionTex,vec2(uv.x                  ,uv.y-0.6*u_PixelSize.y));
	float sum = (c1.x+c2.x+c3.x+c4.x)/4;

	//return color_fxaa(u_OcclusionTex,uv,u_ScreenSize);
	return sum;
}

float get_occlusion(vec2 uv){
    float c1 = get_occlusion_p(vec2(uv.x+0.2*u_PixelSize.x,uv.y                ));
	float c2 = get_occlusion_p(vec2(uv.x                ,uv.y+0.2*u_PixelSize.y));
	float c3 = get_occlusion_p(vec2(uv.x-0.2*u_PixelSize.x,uv.y                ));
	float c4 = get_occlusion_p(vec2(uv.x                ,uv.y-0.2*u_PixelSize.y));
	float sum = (c1+c2+c3+c4)/4;
	return sum;
}
*/

float get_occlusion(vec2 uv){
    float c1 = texture2D(u_OcclusionTex,vec2(uv.x+0.2*u_PixelSize.x,uv.y                )).x;
	float c2 = texture2D(u_OcclusionTex,vec2(uv.x                ,uv.y+0.2*u_PixelSize.y)).x;
	float c3 = texture2D(u_OcclusionTex,vec2(uv.x-0.2*u_PixelSize.x,uv.y                )).x;
	float c4 = texture2D(u_OcclusionTex,vec2(uv.x                ,uv.y-0.2*u_PixelSize.y)).x;
	float sum = (c1+c2+c3+c4)/4;
	return sum;
}

void main_draw(){
    if (u_SuperSampling==1)
        gl_Color = get_color(u_AlbedoSpecTex,uv);
    else
        gl_Color = get_supersampled_color(u_AlbedoSpecTex,uv);

    vec2 vignette_uv = vec2(2*uv.x-1,2*uv.y-1);
    if (u_Vignette!=0)
        gl_Color = get_vignette_color(gl_Color,vec2(2*uv.x-1,2*uv.y-1),u_Vignette);
 // gl_Color*=0.001;
    gl_Color*=u_Brightness;
	gl_Color*=get_occlusion(uv);
	//gl_Color*=texture2D(u_OcclusionTex,uv).r;
}

void occlusion_draw(){
     gl_Color = vec4(texture2D(u_OcclusionTex,uv).r);
     /*gl_Color += vec4(texture(u_OcclusionTex,vec2(uv.x+u_PixelSize.x,uv.y+u_PixelSize.y)).r);
     gl_Color += vec4(texture(u_OcclusionTex,vec2(uv.x-u_PixelSize.x,uv.y+u_PixelSize.y)).r);
     gl_Color += vec4(texture(u_OcclusionTex,vec2(uv.x+u_PixelSize.x,uv.y-u_PixelSize.y)).r);
     gl_Color += vec4(texture(u_OcclusionTex,vec2(uv.x-u_PixelSize.x,uv.y-u_PixelSize.y)).r);

     gl_Color += vec4(texture(u_OcclusionTex,vec2(uv.x+u_PixelSize.x,uv.y)).r);
     gl_Color += vec4(texture(u_OcclusionTex,vec2(uv.x-u_PixelSize.x,uv.y)).r);
     gl_Color += vec4(texture(u_OcclusionTex,vec2(uv.x,uv.y+u_PixelSize.y)).r);
     gl_Color += vec4(texture(u_OcclusionTex,vec2(uv.x,uv.y-u_PixelSize.y)).r);
     gl_Color *= 0.111111;*/
}

void main(){
 // sampler2D u_AlbedoSpecTex = u_AlbedoSpecTex;
    if (u_DrawingMode==0){
        main_draw();
    }else if (u_DrawingMode==1){
        gl_Color = texture2D(u_AlbedoSpecTex,uv);
    }else if (u_DrawingMode==2){
        gl_Color = texture2D(u_NormalTex,uv);
    }else if (u_DrawingMode==3){
        gl_Color = texture2D(u_PositionTex,uv);
    }else if (u_DrawingMode==4){
        gl_Color = texture2D(u_DiffuseTex,uv);
    }else if (u_DrawingMode==5){
       occlusion_draw();
    }
    gl_Color.rgb = pow(gl_Color.rgb,vec3(1.0/u_Gamma));
 // gl_Color *= texture2D(u_NormalTex,uv);
  //gl_Color *= texture2D(u_PositionTex,uv);
  //gl_Color += texture2D(u_DiffuseTex,uv);
  //gl_Color = draw_text(uv,vec2(  floor(u_ScreenSize.x/u_ScreenSize.y)*(u_ScreenSize.y/(5*24)) , floor(u_ScreenSize.y/(32*5))  ),u_ScreenSize);
}









