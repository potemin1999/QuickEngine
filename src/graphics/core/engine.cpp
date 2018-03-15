#include "engine.h"
#include "object.h"
#include "settings.h"
#include "shader.h"

//#include "engine_utils.h"

#define DEBUG_DRAW 1



int supersampling = 1;
int anisotropy = 8;

float vert[] =
        { -1.0f,-1.0f, 0.0f,     -1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f,
          1.0f, 1.0f, 0.0f,      1.0f,-1.0f, 0.0f,   -1.0f,-1.0f, 0.0f };

//int indices[] = { 0,1,2,     2,3,0 };

void Engine::check_gl_error(const char* tag) {
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		printf("ERROR %s : %i \n description: no \n", tag, error);
	}
}

Engine::Engine() {
	log("created new engine\n");
    p_GeometryPass = new GeometryPass();
    p_SSAOPass = new SSAOPass();
}

void Engine::init() {
	int i_eu = EU::init_utils(this);
	log("init engine utils %i \n", i_eu);
	camera = new Camera();
	camera->farZ = 10.0f;
	camera->nearZ = 0.01f;
	camera->fovy = 30.14159f / 3.0f;
	float p[] = { 0.0f,0.0f,0.0f };
	float v[] = { 0.0f,0.0f,1.0f };
	float u[] = { 0.0f,1.0f,0.0f };
	camera->position_camera(p, v, u);

	*v_Vignette = 1.0f;
	*v_Brightness = 1.0f;
	*v_PostBrightness = 1.0f;
	*v_FXAA = 1;
	*v_DrawingMode = 0;
	*v_Gamma = 1.0;
	*v_SSAOSamples = 16;

    GeometryPassInitData geometryPassInitData;
    geometryPassInitData.objects = &objects;
    geometryPassInitData.camera = &camera;
    geometryPassInitData.v_Brightness = &v_Brightness;
    p_GeometryPass->init(&geometryPassInitData);

    SSAOPassInitData ssaoPassInitData;
    ssaoPassInitData.v_SSAOScale = &v_SSAOScale;
    ssaoPassInitData.v_PixelSize = &v_PixelSize;
    ssaoPassInitData.v_SSAOSamples = &v_SSAOSamples;
    ssaoPassInitData.camera = &camera;
    ssaoPassInitData.t_Depthmap = p_GeometryPass->getDepthmap();
    ssaoPassInitData.t_LightingTex = p_GeometryPass->getPositionTex();
    ssaoPassInitData.t_PositionTex = p_GeometryPass->getPositionTex();
    ssaoPassInitData.t_NormalTex = p_GeometryPass->getNormalTex();
    p_SSAOPass->init(&ssaoPassInitData);
}


void Engine::compile_shaders() {
    log("shader compiling started\n");
	p_GeometryPass->compileShaders();
    s_LightShader = new Shader("shaders/light_vertex_shader.glsl","shaders/light_fragment_shader.glsl");
    p_SSAOPass->compileShaders();
    s_PostShader = new Shader("shaders/post_shader_vertex.glsl","shaders/post_shader_fragment.glsl");

	s_LightShader->compile();
    s_PostShader->compile();

    s_LightShader->uniform("u_PixelSize");
    s_LightShader->uniform("u_AlbedoSpecTex");
    s_LightShader->uniform("u_NormalTex");
    s_LightShader->uniform("u_PositionTex");

    s_PostShader->uniform("u_DrawingMode");
    s_PostShader->uniform("u_PixelSize");
    s_PostShader->uniform("u_ScreenSize");
    s_PostShader->uniform("u_SuperSampling");
    s_PostShader->uniform("u_Vignette");
    s_PostShader->uniform("u_FXAA");
    s_PostShader->uniform("u_Gamma");
    s_PostShader->uniform("u_Brightness");
    s_PostShader->uniform("u_AlbedoSpecTex");
    s_PostShader->uniform("u_NormalTex");
    s_PostShader->uniform("u_PositionTex");
    s_PostShader->uniform("u_DiffuseTex");
    s_PostShader->uniform("u_OcclusionTex");

	log("post shader program binded\n");
//#ifdef GRAPHICS_DEBUG
	dump_uniforms();
//#endif
}

void Engine::dump_uniforms() {
    /*
	printf(" main shader uniforms:\n");
	printf("  u_M = %i\n", *u_M);
    printf("  u_V = %i\n", *u_V);
	printf("  u_P = %i\n", *u_P);
	printf("  u_MVP = %i\n", *u_MVP);
	printf("  u_ZFar = %i\n", *u_MainZFar);
	printf("  u_Brightness(main) = %i\n", *u_Brightness);
	printf("  u_MainAmbientTex = %i\n", *u_MainAmbientTex);
	printf("  u_MainNormalTex = %i\n", *u_MainNormalTex);
*/
    /*
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
	printf("  u_PostOcclusionTex = %i\n", *u_PostOcclusionTex);*/
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



    p_GeometryPass->onResize(w,h);
    *t_FBO_depthmap = (*p_GeometryPass->getDepthmap())->getPointer();
    *t_FBO_albedo = (*p_GeometryPass->getAlbedoTex())->getPointer();
    *t_FBO_normal = (*p_GeometryPass->getNormalTex())->getPointer();
    *t_FBO_position = (*p_GeometryPass->getPositionTex())->getPointer();

    p_SSAOPass->onResize(w,h);
	*t_FBO_occlusion = (*p_SSAOPass->getOcclusionTexture())->getPointer();
	/*EU::genFBO(mFBO_geometry, w, h);
	EU::genFBODepthmap(t_FBO_depthmap, w, h, GL_DEPTH_COMPONENT24);
	EU::genFBOTexture(t_FBO_albedo, w, h, GL_COLOR_ATTACHMENT0, GL_LINEAR, GL_NEAREST);
	EU::genFBOTexture(t_FBO_normal, w, h, GL_COLOR_ATTACHMENT1, GL_LINEAR, GL_NEAREST);
	EU::genFBOTexture(t_FBO_position, w, h, GL_COLOR_ATTACHMENT2, GL_LINEAR, GL_NEAREST);
	EU::glUseAttachments(new unsigned[3]{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2 },3);
    */

    log("t_Depthmap : %u \n",*t_FBO_depthmap);
    log("t_Albedo : %u \n",*t_FBO_albedo);
    log("t_Normal : %u \n",*t_FBO_normal);
    log("t_Position : %u \n",*t_FBO_position);

	EU::genFBO(mFBO_lighting, w, h);
	EU::genFBOTexture(t_FBO_diffuse, w, h, GL_COLOR_ATTACHMENT3, GL_LINEAR, GL_NEAREST);
	EU::glUseAttachments(new unsigned[1]{GL_COLOR_ATTACHMENT3 }, 1);

	//EU::genFBO(mFBO_occlusion, v_SSAOSize[0], v_SSAOSize[1]);
	//EU::genSSAOTexture(t_FBO_occlusion, v_SSAOSize[0], v_SSAOSize[1], GL_COLOR_ATTACHMENT4, GL_LINEAR, GL_LINEAR);
	//EU::glUseAttachments(new unsigned[1]{ GL_COLOR_ATTACHMENT4 }, 1);

	//EU::glUseAttachments(new unsigned[4]{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3 }, 4);

	if (supersampling != 1) {
		w /= supersampling; h /= supersampling;
	}


    unsigned int post_vbo;
    glGenBuffers(1, &post_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, post_vbo);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), vert, GL_STATIC_DRAW);
    glGenVertexArrays(1,mPostVAO);
    glBindVertexArray(*mPostVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

	//EU::genVBOandVAO(vert, 12 * sizeof(float), mPostVAO);

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
	//glBindFramebuffer(GL_FRAMEBUFFER, *mFBO_geometry);
    p_GeometryPass->draw();
	//draw_scene();



	if (*v_DrawingMode == 0 | *v_DrawingMode == 5) {
		if (supersampling != 1)
			glViewport(0, 0, supersampling*width, supersampling*height);
        p_SSAOPass->draw();
		//glBindFramebuffer(GL_FRAMEBUFFER, *mFBO_occlusion);
		//draw_ssao();
	}


    if (*v_DrawingMode != 5) {
        if (supersampling != 1)
            glViewport(0, 0, supersampling*width, supersampling*height);
        glBindFramebuffer(GL_FRAMEBUFFER, *mFBO_lighting);
        draw_light();
    }
	//glFlush();

	if (supersampling != 1)
		glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	post_draw();
	//draw_scene();
	glFlush();
}


void Engine::post_draw() {
    s_PostShader->use();
	//SH::use_shader_program(s_PostShader);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    s_PostShader->uniform1i("u_AlbedoSpecTex",0);
	//glUniform1i(*u_PostAlbedoSpecTex, 0);
	glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D,*t_FBO_albedo);
	glBindTexture(GL_TEXTURE_2D,(*p_GeometryPass->getAlbedoTex())->getPointer());
    s_PostShader->uniform1i("u_NormalTex",1);
	//glUniform1i(*u_PostNormalTex, 1);
	glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D,*t_FBO_normal);
	glBindTexture(GL_TEXTURE_2D,(*p_GeometryPass->getNormalTex())->getPointer());
    s_PostShader->uniform1i("u_PositionTex",2);
	//glUniform1i(*u_PostPositionTex, 2);
	glActiveTexture(GL_TEXTURE2);
    //glBindTexture(GL_TEXTURE_2D,*t_FBO_position);
	glBindTexture(GL_TEXTURE_2D,(*p_GeometryPass->getPositionTex())->getPointer());
    s_PostShader->uniform1i("u_DiffuseTex",3);
	//glUniform1i(*u_PostDiffuseTex, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D,(*p_GeometryPass->getAlbedoTex())->getPointer());
    s_PostShader->uniform1i("u_OcclusionTex",4);
	//glUniform1i(*u_PostOcclusionTex, 4);
	glActiveTexture(GL_TEXTURE4);
	//glBindTexture(GL_TEXTURE_2D, *t_FBO_occlusion);
    glBindTexture(GL_TEXTURE_2D, (*p_SSAOPass->getOcclusionTexture())->getPointer());

	glBindVertexArray(*mPostVAO);
	glEnableVertexAttribArray(0);
    s_PostShader->uniform2f("u_PixelSize",v_PixelSize[0], v_PixelSize[1]);
    s_PostShader->uniform2f("u_ScreenSize",v_ScreenSize[0], v_ScreenSize[1]);
    s_PostShader->uniform1i("u_SuperSampling",supersampling);
    s_PostShader->uniform1f("u_Vignette",*v_Vignette);
    s_PostShader->uniform1f("u_Brightness",*v_PostBrightness);
    s_PostShader->uniform1i("u_FXAA",*v_FXAA);
    s_PostShader->uniform1f("u_Gamma",*v_Gamma);
    s_PostShader->uniform1i("u_DrawingMode",*v_DrawingMode);
	/*glUniform2f(*u_PostPixelSize, v_PixelSize[0], v_PixelSize[1]);
	glUniform2f(*u_ScreenSize, v_ScreenSize[0], v_ScreenSize[1]);
	glUniform1i(*u_SuperSampling, supersampling);
	glUniform1f(*u_Vignette, *v_Vignette);
	glUniform1f(*u_PostBrightness, *v_PostBrightness);
	glUniform1i(*u_FXAA, *v_FXAA);
	glUniform1f(*u_Gamma, *v_Gamma);
	glUniform1i(*u_DrawingMode, *v_DrawingMode);
    */
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);

	glBindVertexArray(0);
}



void Engine::draw_ssao() {
    /*s_SSAOShader->use();
	//SH::use_shader_program(s_SSAOShader);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    s_SSAOShader->uniform1i("u_DepthTex",0);
	//glUniform1i(*u_SSAODepthTex, 0);
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,*t_FBO_depthmap);
	//glBindTexture(GL_TEXTURE_2D, p_GeometryPass->getDepthmap()->getPointer());
    s_SSAOShader->uniform1i("u_NormalTex",1);
	//glUniform1i(*u_SSAONormalTex, 1);
	glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,*t_FBO_normal);
	//glBindTexture(GL_TEXTURE_2D, p_GeometryPass->getNormalTex()->getPointer());
    s_SSAOShader->uniform1i("u_PositionTex",2);
	//glUniform1i(*u_SSAOPositionTex, 2);
	glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, *t_FBO_position);
	//glBindTexture(GL_TEXTURE_2D, p_GeometryPass->getPositionTex()->getPointer());
    s_SSAOShader->uniform1i("u_LightingTex",3);
	//glUniform1i(*u_SSAOLightingTex, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, *t_FBO_position);

	glBindVertexArray(*mPostVAO);
	glEnableVertexAttribArray(0);
    s_SSAOShader->uniformMatrix4fv("u_Projection",1,0,camera->value_ptr(camera->mProjectionMatrix));
    s_SSAOShader->uniform1f("u_ZFar",camera->farZ);
    s_SSAOShader->uniform1f("u_Aspect",camera->aspect);
    s_SSAOShader->uniform1f("u_Fov",camera->fovy);
    s_SSAOShader->uniform1i("u_Samples",*v_SSAOSamples);
    s_SSAOShader->uniform3fv("u_RandomMap", *v_SSAOSamples,v_SSAORandomMap);
    s_SSAOShader->uniform2f("u_PixelSize",v_PixelSize[0], v_PixelSize[1]);
    s_SSAOShader->uniform2f("u_Scale", v_SSAOScale[0], v_SSAOScale[1]);
	/*glUniformMatrix4fv(*u_SSAOProjection, 1, GL_FALSE, camera->value_ptr(camera->mProjectionMatrix));
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

	glBindVertexArray(0);*/
}

void Engine::draw_light() {
    s_LightShader->use();
	//SH::use_shader_program(s_LightShader);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    s_LightShader->uniform1i("u_AlbedoSpecTex",0);
	//glUniform1i(*u_LightAlbedoSpecTex, 0);
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, *t_FBO_albedo);
	//glBindTexture(GL_TEXTURE_2D, p_GeometryPass->getAlbedoTex()->getPointer());
    s_LightShader->uniform1i("u_NormalTex",1);
	//glUniform1i(*u_LightNormalTex, 1);
	glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, *t_FBO_normal);
	//glBindTexture(GL_TEXTURE_2D, p_GeometryPass->getNormalTex()->getPointer());
    s_LightShader->uniform1i("u_PositionTex",2);
	//glUniform1i(*u_LightPositionTex, 2);
	glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, *t_FBO_position);
	//glBindTexture(GL_TEXTURE_2D, p_GeometryPass->getPositionTex()->getPointer());

	glBindVertexArray(*mPostVAO);
	glEnableVertexAttribArray(0);
    s_LightShader->uniform2f("u_PixelSize", v_PixelSize[0], v_PixelSize[1]);
	//glUniform2f(*u_LightPixelSize, v_PixelSize[0], v_PixelSize[1]);
	//glUniform2f(*u_ScreenSize, v_ScreenSize[0], v_ScreenSize[1]);
	//glUniform1i(*u_SuperSampling, supersampling);
	//glUniform1f(*u_Vignette, *v_Vignette);
	//glUniform1f(*u_PostBrightness, *v_PostBrightness);
	//glUniform1i(*u_FXAA, *v_FXAA);
	//glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);

	glBindVertexArray(0);
}

void Engine::draw_scene() {
	//check_gl_error("pre draw");
	//SH::use_shader_program(s_MainShader);
    p_GeometryPass->draw();
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
