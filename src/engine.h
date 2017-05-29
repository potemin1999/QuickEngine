#pragma once

#include "glad\glad.h"
#include "camera.h"
#include <list>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "object.h"
#include "material.h"
#include "texture.h"
#include "shader.h"
#include "material.h"
#include "camera.h"
#include "mesh.h"

using namespace std;







class Engine {
public:
	Engine();
	Camera* camera;
	vector<Object*> objects;
	vector<Material*> materials;
	vector<Texture*> textures;

	Shader* s_MainShader;
	Shader* s_PostShader;
	Shader* s_LightShader;
	Shader* s_SSAOShader;

	unsigned int* mPostVAO = new unsigned int;
	unsigned int* mPostVBO_vert = new unsigned;
	unsigned int* mPostVBO_ind = new unsigned;

	unsigned int* mFBO_geometry = new unsigned;
	unsigned int* mFBO_lighting = new unsigned;
	unsigned int* mFBO_occlusion = new unsigned;
	//unsigned int* mFBO_tex = new unsigned[2];

	//unsigned int* mFBO_accum = new unsigned;
	unsigned int* t_FBO_depthmap = new unsigned;
	unsigned int* t_FBO_normal = new unsigned;
	unsigned int* t_FBO_albedo = new unsigned;
	unsigned int* t_FBO_diffuse = new unsigned;
	unsigned int* t_FBO_position = new unsigned;
	unsigned int* t_FBO_occlusion = new unsigned;
	//int *m_MainPassProgram = new int;
	//int *m_PostPassProgram = new int;
	float* v_PixelSize = new float[2];
	float* v_ScreenSize = new float[2];
	float* v_SSAOScale = new float[2];
	int* v_SSAOSize = new int[2];
	int* v_SSAOSamples = new int;
	int* v_SSAOPower = new int;
	int* v_SSAORadius = new int;
	float* v_SSAORandomMap = new float;

	float* v_Vignette = new float;
	float* v_Brightness = new float;
	float* v_PostBrightness = new float;
	int* v_DrawingMode = new int;
	int* v_FXAA = new int;
	float* v_Gamma = new float;

	unsigned int* u_FXAA = new unsigned;
	unsigned int* u_PostPixelSize = new unsigned;
	unsigned int* u_LightPixelSize = new unsigned;
	unsigned int* u_SSAOPixelSize = new unsigned;
	unsigned int* u_SSAOSamples = new unsigned;
	unsigned int* u_ScreenSize = new unsigned;
	unsigned int* u_SuperSampling = new unsigned;
	unsigned int* u_Vignette = new unsigned;
	unsigned int* u_Brightness = new unsigned;
	unsigned int* u_DrawingMode = new unsigned;
	unsigned int* u_PostBrightness = new unsigned;
	unsigned int* u_Gamma = new unsigned;
	unsigned int* u_SSAOProjection = new unsigned;
	unsigned int* u_SSAOFov = new unsigned;
	unsigned int* u_SSAOAspect = new unsigned;
	unsigned int* u_SSAOScale = new unsigned;

	unsigned int* u_LightAlbedoSpecTex = new unsigned;
	unsigned int* u_LightNormalTex = new unsigned;
	unsigned int* u_LightPositionTex = new unsigned;

	unsigned int* u_PostAlbedoSpecTex = new unsigned;
	unsigned int* u_PostNormalTex = new unsigned;
	unsigned int* u_PostPositionTex = new unsigned;
	unsigned int* u_PostDiffuseTex = new unsigned;
	unsigned int* u_PostOcclusionTex = new unsigned;

	unsigned int* u_RandomMap = new unsigned;
	unsigned int* u_SSAODepthTex = new unsigned;
	unsigned int* u_SSAONormalTex = new unsigned;
	unsigned int* u_SSAOPositionTex = new unsigned;
	unsigned int* u_SSAOLightingTex = new unsigned;

	unsigned int* u_MVP = new unsigned;
	unsigned int* u_V = new unsigned;
	unsigned int* u_P = new unsigned;
	unsigned int* u_M = new unsigned;
	unsigned int* u_MainZFar = new unsigned;
	unsigned int* u_SSAOZFar = new unsigned;
	unsigned int* u_MainAmbientTex = new unsigned;
	unsigned int* u_MainNormalTex = new unsigned;


	int width = 10;
	int height = 10;
	float forward_speed = 2.8f;
	float backward_speed = 1.8f;
	float strafe_speed = 2.2f;

	bool request_up = false;
	bool request_down = false;
	bool request_left = false;
	bool request_right = false;
	bool request_forward = false;
	bool request_backward = false;

	void init();
	void compile_shaders();
	void create_world();
	void resize(int w,int h);
	void draw(float dT);
	void draw_scene();
	void draw_light();
	void draw_ssao();
	void post_draw();
	void add_object(Object* o);
	void dump_uniforms();
	float random(float min, float max);
	static Engine* get_instance();
	static void check_gl_error(const char* tag);
private:
	float speed_up = 0;
	float speed_forward = 0;
	float speed_right = 0;
};



class EU {
public:
	static int init_utils(Engine* e);
	static string load_string(char* filename);
	static void genVBOandVAO(float arr[], int len, int &vertVAO);
	static void genFBO(unsigned int* mFB,int width, int height);
	static void genFBODepthmap(unsigned int* handle, int width, int height, unsigned precision);
	static void genFBOTexture(unsigned int* handle,int width, int height,unsigned attachment,int min_f,int max_f);
	static void genSSAOTexture(unsigned int* handle, int width, int height, unsigned attachment, int min_f, int max_f);
	static void glUseAttachments(const unsigned* attachments,int size);
	static void render_objects_compute(Engine* e);
	static void create_objects(Engine* e);
};


