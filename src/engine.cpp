#include "engine.h"
#include "object.h"
#include "settings.h"

//#include "engine_utils.h"

#define DEBUG_DRAW 1

float vert[] =
{ -1,-1, 0,     -1, 1, 0,    1, 1, 0,
1, 1, 0,      1,-1, 0,   -1,-1, 0 };

int supersampling = 1;
int anisotropy = 8;

//int indices[] = { 0,1,2,     2,3,0 };

void Engine::check_gl_error(const char* tag) {
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		printf("ERROR %s : %i \n description: no \n", tag, error);
	}
}

Engine::Engine() {
	printf("created new engine\n");
}

void Engine::init() {
	int i_eu = EU::init_utils(this);
	printf("init engine utils %i \n", i_eu);
	camera = new Camera();
	camera->farZ = 10.0f;
	camera->nearZ = 0.01f;
	camera->fovy = 30.14159f / 3.0f;
	float p[] = { 0.0f,0.0f,0.0f };
	float v[] = { 0.0f,0.0f,1.0f };
	float u[] = { 0.0f,1.0f,0.0f };
	camera->position_camera(p, v, u);

}


void Engine::compile_shaders() {
	s_MainShader = SH::create_shader_program("shaders\\shader_vertex.glsl", "shaders\\shader_fragment.glsl");
	//*m_MainPassProgram = SH::load_shaders("shaders\\shader_vertex.glsl", "shaders\\shader_fragment.glsl");
	//printf("init base shader program %i\n", *m_MainPassProgram);
	//SH::bind_shader_program(this, *m_MainPassProgram);
	//*m_MainPassProgram = program_handle;
	check_gl_error("main shader compiler");
	SH::get_uniform(s_MainShader, "u_V", u_V);
	SH::get_uniform(s_MainShader, "u_P", u_P);
	SH::get_uniform(s_MainShader, "u_M", u_M);
	SH::get_uniform(s_MainShader, "u_MVP", u_MVP);
	SH::get_uniform(s_MainShader, "u_ZFar", u_MainZFar);
	SH::get_uniform(s_MainShader, "u_AmbientTex", u_MainAmbientTex);
	SH::get_uniform(s_MainShader, "u_NormalTex", u_MainNormalTex);
	SH::get_uniform(s_MainShader, "u_Brightness", u_Brightness);

	printf("main shader program binded\n");

	s_LightShader = SH::create_shader_program("shaders\\light_vertex_shader.glsl", "shaders\\light_fragment_shader.glsl");
	SH::get_uniform(s_LightShader, "u_PixelSize", u_LightPixelSize);
	SH::get_uniform(s_LightShader, "u_AlbedoSpecTex", u_LightAlbedoSpecTex);
	SH::get_uniform(s_LightShader, "u_NormalTex", u_LightNormalTex);
	SH::get_uniform(s_LightShader, "u_PositionTex", u_LightPositionTex);

	s_SSAOShader = SH::create_shader_program("shaders\\ssao_vertex_shader.glsl", "shaders\\ssao_fragment_shader.glsl");
	SH::get_uniform(s_SSAOShader, "u_RandomMap", u_RandomMap);
	SH::get_uniform(s_SSAOShader, "u_Projection", u_SSAOProjection);
	SH::get_uniform(s_SSAOShader, "u_Aspect", u_SSAOAspect);
	SH::get_uniform(s_SSAOShader, "u_Fov", u_SSAOFov);
	SH::get_uniform(s_SSAOShader, "u_ZFar", u_SSAOZFar);
	SH::get_uniform(s_SSAOShader, "u_Samples", u_SSAOSamples);
	SH::get_uniform(s_SSAOShader, "u_Scale", u_SSAOScale);
	SH::get_uniform(s_SSAOShader, "u_PixelSize", u_SSAOPixelSize);
	SH::get_uniform(s_SSAOShader, "u_DepthTex", u_SSAODepthTex);
	SH::get_uniform(s_SSAOShader, "u_NormalTex", u_SSAONormalTex);
	SH::get_uniform(s_SSAOShader, "u_PositionTex", u_SSAOPositionTex);
	SH::get_uniform(s_SSAOShader, "u_LightingTex", u_SSAOLightingTex);

	s_PostShader = SH::create_shader_program("shaders\\post_shader_vertex.glsl", "shaders\\post_shader_fragment.glsl");
	//*m_PostPassProgram = SH::load_shaders("shaders\\post_shader_vertex.glsl", "shaders\\post_shader_fragment.glsl");
	//printf("init post shader program %i\n", *m_PostPassProgram);
	SH::get_uniform(s_PostShader, "u_DrawingMode", u_DrawingMode);
	SH::get_uniform(s_PostShader, "u_PixelSize", u_PostPixelSize);
	SH::get_uniform(s_PostShader, "u_ScreenSize", u_ScreenSize);
	SH::get_uniform(s_PostShader, "u_SuperSampling", u_SuperSampling);
	SH::get_uniform(s_PostShader, "u_Vignette", u_Vignette);
	SH::get_uniform(s_PostShader, "u_FXAA", u_FXAA);
	SH::get_uniform(s_PostShader, "u_Gamma", u_Gamma);
	SH::get_uniform(s_PostShader, "u_Brightness", u_PostBrightness);
	SH::get_uniform(s_PostShader, "u_AlbedoSpecTex", u_PostAlbedoSpecTex);
	SH::get_uniform(s_PostShader, "u_NormalTex", u_PostNormalTex);
	SH::get_uniform(s_PostShader, "u_PositionTex", u_PostPositionTex);
	SH::get_uniform(s_PostShader, "u_DiffuseTex", u_PostDiffuseTex);
	SH::get_uniform(s_PostShader, "u_OcclusionTex", u_PostOcclusionTex);

	//*u_PixelSize = glGetUniformLocation(*m_PostPassProgram, "u_PixelSize");
	////*u_ScreenSize = glGetUniformLocation(*m_PostPassProgram, "u_ScreenSize");
	//*u_SuperSampling = glGetUniformLocation(*m_PostPassProgram, "u_SuperSampling");
	//*u_Vignette = glGetUniformLocation(*m_PostPassProgram, "u_Vignette");
	//SH::bind_shader_program(this, *m_PostPassProgram);
	printf("post shader program binded\n");
	dump_uniforms();
}

void Engine::dump_uniforms() {
	printf(" main shader uniforms:\n");
	printf("  u_M = %i\n", *u_M);
	printf("  u_V = %i\n", *u_V);
	printf("  u_P = %i\n", *u_P);
	printf("  u_MVP = %i\n", *u_MVP);
	printf("  u_ZFar = %i\n", *u_MainZFar);
	printf("  u_Brightness(main) = %i\n", *u_Brightness);
	printf("  u_MainAmbientTex = %i\n", *u_MainAmbientTex);
	printf("  u_MainNormalTex = %i\n", *u_MainNormalTex);

	printf(" light shader uniforms:\n");
	//printf("  u_MVP = %i\n", *u_MVP);
	printf("  u_LightAlbedoSpecTex = %i\n", *u_LightAlbedoSpecTex);
	printf("  u_LightNormalTex = %i\n", *u_LightNormalTex);
	printf("  u_LightPositionTex = %i\n", *u_LightPositionTex);

	printf(" ssao shader uniforms:\n");
	//printf("  u_MVP = %i\n", *u_MVP);
	printf("  u_ZFar = %i\n", *u_SSAOZFar);
	printf("  u_SSAOSamples = %i\n", *u_SSAOSamples);
	printf("  u_SSAOScale = %i\n", *u_SSAOScale);
	printf("  u_SSAOProjection = %i\n", *u_SSAOProjection);
	printf("  u_SSAOFov = %i\n", *u_SSAOFov);
	printf("  u_SSAOAspect = %i\n", *u_SSAOAspect);
	printf("  u_RandomMap = %i\n", *u_RandomMap);
	printf("  u_SSAODepthTex = %i\n", *u_SSAODepthTex);
	printf("  u_SSAONormalTex = %i\n", *u_SSAONormalTex);
	printf("  u_SSAOPositionTex = %i\n", *u_SSAOPositionTex);
	printf("  u_SSAOLightingTex = %i\n", *u_SSAOLightingTex);

	printf(" post shader uniforms:\n");
	printf("  u_PixelSize = %i\n", *u_PostPixelSize);
	printf("  u_ScreenSize = %i\n", *u_ScreenSize);
	printf("  u_SuperSampling = %i\n", *u_SuperSampling);
	printf("  u_Vignette = %i\n", *u_Vignette);
	printf("  u_FXAA = %i\n", *u_FXAA);
	printf("  u_Gamma = %i\n", *u_Gamma);
	printf("  u_Brightness(post) = %i\n", *u_Brightness);
	printf("  u_PostAlbedoSpecTex = %i\n", *u_PostAlbedoSpecTex);
	printf("  u_PostNormalTex = %i\n", *u_PostNormalTex);
	printf("  u_PostPositionTex = %i\n", *u_PostPositionTex);
	printf("  u_PostDiffuseTex = %i\n", *u_PostDiffuseTex);
	printf("  u_PostOcclusionTex = %i\n", *u_PostOcclusionTex);
}

void Engine::create_world() {
	EU::create_objects(this);
}

void Engine::resize(int w, int h) {
	if (h == 0) h = 10;
	if (w == 0) w = 10;
	width = w;
	height = h;
	glViewport(0, 0, width, height);
	if (supersampling != 1) {
		w *= supersampling; h *= supersampling;
	}
	camera->set_perspective(((float)w) / ((float)h), 1.0f, 40.0f);
	int m_offset = sizeof(unsigned);
	v_PixelSize[0] = 1.0f / ((float)(w));
	v_PixelSize[1] = 1.0f / ((float)(h));
	v_ScreenSize[0] = (float)w;
	v_ScreenSize[1] = (float)h;
	v_SSAOSize[0] = w;
	v_SSAOSize[1] = h;
	v_SSAOScale[0] = v_ScreenSize[0] / v_SSAOSize[0];
	v_SSAOScale[1] = v_ScreenSize[1] / v_SSAOSize[1];
	*v_Vignette = 1.0f;
	*v_Brightness = 1.0f;
	*v_PostBrightness = 1.0f;
	*v_FXAA = 1;
	*v_DrawingMode = 0;
	*v_Gamma = 1.0;
	*v_SSAOSamples = 64;
	v_SSAORandomMap = new float[3 * *v_SSAOSamples];

	printf("random map:\n");
	for (int i = 0; i<*v_SSAOSamples; i++) {
		float x = random(-1, 1);
		float y = random(-1, 1);
		float z = random(-1, 0);
		float n = Camera::sqrt_inv(x*x + y*y + z*z);// *((i + 1.0f) / *v_SSAOSamples);
		float m = ((i + 1.0f) / (*v_SSAOSamples));// * ((i + 1.0f) / (*v_SSAOSamples)) ;
		v_SSAORandomMap[3 * i + 0] = x*n*m;
		v_SSAORandomMap[3 * i + 1] = y*n*m;
		v_SSAORandomMap[3 * i + 2] = z*n*m-0.02f;
		printf("( %f , %f , %f )\n", x*n*m, y*n*m, z*n*m);
	}

	EU::genFBO(mFBO_geometry, w, h);
	EU::genFBODepthmap(t_FBO_depthmap, w, h, GL_DEPTH_COMPONENT24);
	EU::genFBOTexture(t_FBO_albedo, w, h, GL_COLOR_ATTACHMENT0, GL_LINEAR, GL_NEAREST);
	EU::genFBOTexture(t_FBO_normal, w, h, GL_COLOR_ATTACHMENT1, GL_LINEAR, GL_NEAREST);
	EU::genFBOTexture(t_FBO_position, w, h, GL_COLOR_ATTACHMENT2, GL_LINEAR, GL_NEAREST);
	EU::glUseAttachments(new unsigned[3]{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2 },3);


	EU::genFBO(mFBO_lighting, w, h);
	EU::genFBOTexture(t_FBO_diffuse, w, h, GL_COLOR_ATTACHMENT3, GL_LINEAR, GL_NEAREST);
	EU::glUseAttachments(new unsigned[1]{GL_COLOR_ATTACHMENT3 }, 1);

	EU::genFBO(mFBO_occlusion, v_SSAOSize[0], v_SSAOSize[1]);
	EU::genSSAOTexture(t_FBO_occlusion, v_SSAOSize[0], v_SSAOSize[1], GL_COLOR_ATTACHMENT4, GL_LINEAR, GL_LINEAR);
	EU::glUseAttachments(new unsigned[1]{ GL_COLOR_ATTACHMENT4 }, 1);

	//EU::glUseAttachments(new unsigned[4]{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3 }, 4);

	if (supersampling != 1) {
		w /= supersampling; h /= supersampling;
	}

	unsigned int post_vbo;
	glGenBuffers(1, &post_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, post_vbo);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), vert, GL_STATIC_DRAW);


	glGenVertexArrays(1, mPostVAO);
	glBindVertexArray(*mPostVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	//EU::genVBOandVAO(vert, 12 * sizeof(float), *mPostVAO);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));

}

void Engine::draw(float dT) {
	if (request_forward & !request_backward) camera->move_camera(forward_speed*dT);
	if (request_backward & !request_forward) camera->move_camera(-backward_speed*dT);
	if (request_right & !request_left) camera->move_strafe(strafe_speed*dT);
	if (request_left & !request_right) camera->move_strafe(-strafe_speed*dT);
	if (request_up & !request_down) camera->move_camera_up(dT);
	if (request_down & !request_up) camera->move_camera_up(-dT);
	camera->look();

	if (supersampling != 1)
		glViewport(0, 0, supersampling*width, supersampling*height);
	glBindFramebuffer(GL_FRAMEBUFFER, *mFBO_geometry);
	draw_scene();

	if (*v_DrawingMode != 5) {
		if (supersampling != 1)
			glViewport(0, 0, supersampling*width, supersampling*height);
		glBindFramebuffer(GL_FRAMEBUFFER, *mFBO_lighting);
		draw_light();
	}

	if (*v_DrawingMode == 0 | *v_DrawingMode == 5) {
		if (supersampling != 1)
			glViewport(0, 0, supersampling*width, supersampling*height);
		glBindFramebuffer(GL_FRAMEBUFFER, *mFBO_occlusion);
		draw_ssao();
	}
	//glFlush();

	if (supersampling != 1)
		glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	post_draw();
	//draw_scene();*/
	glFlush();
}


void Engine::post_draw() {
	SH::use_shader_program(s_PostShader);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniform1i(*u_PostAlbedoSpecTex, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,*t_FBO_albedo);
	glUniform1i(*u_PostNormalTex, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,*t_FBO_normal);
	glUniform1i(*u_PostPositionTex, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,*t_FBO_position);
	glUniform1i(*u_PostDiffuseTex, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D,*t_FBO_diffuse);
	glUniform1i(*u_PostOcclusionTex, 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, *t_FBO_occlusion);

	glBindVertexArray(*mPostVAO);
	glEnableVertexAttribArray(0);
	glUniform2f(*u_PostPixelSize, v_PixelSize[0], v_PixelSize[1]);
	glUniform2f(*u_ScreenSize, v_ScreenSize[0], v_ScreenSize[1]);
	glUniform1i(*u_SuperSampling, supersampling);
	glUniform1f(*u_Vignette, *v_Vignette);
	glUniform1f(*u_PostBrightness, *v_PostBrightness);
	glUniform1i(*u_FXAA, *v_FXAA);
	glUniform1f(*u_Gamma, *v_Gamma);
	glUniform1i(*u_DrawingMode, *v_DrawingMode);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);

	glBindVertexArray(0);
}



void Engine::draw_ssao() {
	SH::use_shader_program(s_SSAOShader);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniform1i(*u_SSAODepthTex, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *t_FBO_depthmap);
	glUniform1i(*u_SSAONormalTex, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *t_FBO_normal);
	glUniform1i(*u_SSAOPositionTex, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, *t_FBO_position);
	glUniform1i(*u_SSAOLightingTex, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, *t_FBO_position);

	glBindVertexArray(*mPostVAO);
	glEnableVertexAttribArray(0);
	glUniformMatrix4fv(*u_SSAOProjection, 1, GL_FALSE, camera->value_ptr(camera->mProjectionMatrix));
	glUniform1f(*u_SSAOZFar, camera->farZ);
	glUniform1f(*u_SSAOAspect, camera->aspect);
	glUniform1f(*u_SSAOFov, camera->fovy);
	glUniform1i(*u_SSAOSamples, *v_SSAOSamples);
	glUniform3fv(*u_RandomMap, *v_SSAOSamples,v_SSAORandomMap);
	glUniform2f(*u_SSAOPixelSize, v_PixelSize[0], v_PixelSize[1]);
	glUniform2f(*u_SSAOScale, v_SSAOScale[0], v_SSAOScale[1]);
	//glUniform2f(*u_ScreenSize, v_ScreenSize[0], v_ScreenSize[1]);
	//glUniform1i(*u_SuperSampling, supersampling);
	//glUniform1f(*u_Vignette, *v_Vignette);
	//glUniform1f(*u_PostBrightness, *v_PostBrightness);
	//glUniform1i(*u_FXAA, *v_FXAA);
	//glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//glEnable(GL_DEPTH_TEST);
	glDisableVertexAttribArray(0);

	glBindVertexArray(0);
}

void Engine::draw_light() {
	SH::use_shader_program(s_LightShader);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniform1i(*u_LightAlbedoSpecTex, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *t_FBO_albedo);
	glUniform1i(*u_LightNormalTex, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *t_FBO_normal);
	glUniform1i(*u_LightPositionTex, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, *t_FBO_position);

	glBindVertexArray(*mPostVAO);
	glEnableVertexAttribArray(0);
	glUniform2f(*u_LightPixelSize, v_PixelSize[0], v_PixelSize[1]);
	//glUniform2f(*u_ScreenSize, v_ScreenSize[0], v_ScreenSize[1]);
	//glUniform1i(*u_SuperSampling, supersampling);
	//glUniform1f(*u_Vignette, *v_Vignette);
	//glUniform1f(*u_PostBrightness, *v_PostBrightness);
	//glUniform1i(*u_FXAA, *v_FXAA);
	//glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//glEnable(GL_DEPTH_TEST);
	glDisableVertexAttribArray(0);

	glBindVertexArray(0);
}

void Engine::draw_scene() {
	//check_gl_error("pre draw");
	SH::use_shader_program(s_MainShader);
	//glUseProgram(*m_MainPassProgram);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//check_gl_error("draw");
	glUniform1f(*u_MainZFar, camera->farZ);
	glUniform1f(*u_Brightness, *v_Brightness);
	for (int j=0;j<objects.size();j++) {
	    Object* o = objects[j];
	    //objects.get(j);
		for (int i = 0; i < o->mesh_count; i++) {
			try {
				Mesh* m = (o->meshes + i);
				//char* name = m->name;
				//if (m==nullptr)
				//	printf("draw mesh NULLPTR\n");
				//else
				//printf("draw mesh %i\n", m->vert_count);
				//printf("draw mesh %s %i\n", m->name, m->vert_count);
				//glEnableVertexAttribArray(0);
				//glBindBuffer(GL_ARRAY_BUFFER, o->data);
				//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

				glUniformMatrix4fv(*u_MVP, 1, GL_FALSE, camera->value_ptr(camera->mVPMatrix*o->mModelMatrix));
				glUniformMatrix4fv(*u_V, 1, GL_FALSE, camera->value_ptr(camera->mVMatrix));
				glUniformMatrix4fv(*u_P, 1, GL_FALSE, camera->value_ptr(camera->mProjectionMatrix));
				glUniformMatrix4fv(*u_M, 1, GL_FALSE, camera->value_ptr(o->mModelMatrix));
				if (true) {
					glUniform1i(*u_MainAmbientTex, 0);
					glActiveTexture(GL_TEXTURE0);
					if ( m->material->t_Ambient == nullptr) {
						//printf("ERROR: mesh object %s has null ambient texture\n", m->name);
						continue;
					}else {
						glBindTexture(GL_TEXTURE_2D, m->material->t_Ambient->id);
						//printf(" ambient texture binded on link %i\n", m->material->t_Ambient->id);
					}
					glUniform1i(*u_MainNormalTex, 1);
					glActiveTexture(GL_TEXTURE1);
					if (m->material->t_Normal != nullptr) {
						glBindTexture(GL_TEXTURE_2D, m->material->t_Normal->id);
						//printf(" normal texture binded on link %i\n", m->material->t_Ambient->id);
					}else {
						glBindTexture(GL_TEXTURE_2D, m->material->t_Ambient->id);
						//printf(" normal texture binded on link %i\n", m->material->t_Ambient->id);
					}
				}

				glBindVertexArray(m->data);
				//printf("data linked at %i\n", m->data);
				glEnableVertexAttribArray(0);
				glEnableVertexAttribArray(1);
				glEnableVertexAttribArray(2);


				//textureManager->BindTexture(texID);
				glDrawArrays(GL_TRIANGLES, 0, m->vert_count);
				//printf("vert count %i\n", o->vert_count);
				glDisableVertexAttribArray(0);
				glDisableVertexAttribArray(1);
				glDisableVertexAttribArray(2);
				glBindVertexArray(0);
			}
			catch (...) {
			}
		}
	}
	//check_gl_error("post draw");

}

void Engine::add_object(Object* o) {
	objects.push_back(o);
}

Engine* Engine::get_instance() {
	static Engine* engine;
	if (!engine) {
		engine = new Engine();
	}
	return engine;
}
