#define ENGINE_UTILS_INIT_SUCCEED 0x00000000

#include "string_helper.h"
#include <time.h>
#include <direct.h>
#include <fstream>
#include "engine.h"
#include "glad\glad.h"
#include "test_model.h"
#include "max_model.h"



float Engine::random(float min,float max) {
	//srand(time(0));

	float r = (rand()*1.0f)/(RAND_MAX*1.0f);
	return min + (max - min)*r;
}

int EU::init_utils(Engine* e) {
	e->check_gl_error("eu pre init");
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	glClearColor(0, 0, 0, 1);

	glDepthFunc(GL_LEQUAL);
	//glEnable(GL_TEXTURE_2D);
	//glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (anisotropy>1) {
		float* f_max = new float;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,f_max);
	}
	e->check_gl_error("eu init");
	return ENGINE_UTILS_INIT_SUCCEED;
}



string EU::load_string(char* filename) {
	char path[1024];
	_getcwd(path, sizeof(path));
	string _path = string(path);
	_path.append("\\");
	_path.append(filename);
	ifstream file(_path);
	//ifstream file(filename);
	if (!file.is_open()) {
		printf("Error loading file %s\n", _path.data());
		return "";
	}
	std::string str((std::istreambuf_iterator<char>(file)),   std::istreambuf_iterator<char>());
	printf("read file %s\n", _path.data());
	//printf("FILE CONTENT : %s",str.c_str());
	return str;
}



struct Vertex {
	float position[3];
	float uv[2];
	float normal[3];
};

void EU::genVBOandVAO(float arr[], int size, int &verts) {
	unsigned int vbo1;
	//Engine::check_gl_error("pre gen vbo and vao");
	glGenBuffers(1, &vbo1);
	//Engine::check_gl_error("gen vbo and vao");
	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	//Engine::check_gl_error("bind vbo and vao");
	glBufferData(GL_ARRAY_BUFFER, size, arr, GL_STATIC_DRAW);
	//Engine::check_gl_error("bind data vbo and vao");
	unsigned int vao1;
	glGenVertexArrays(1, &vao1);
	glBindVertexArray(vao1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float)*3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float)*5));
	glBindVertexArray(0);
	printf("created vbo and vao with size %i, array on address %p\n",size,&verts);
	Engine::check_gl_error("get vbo and vao");
	verts = vao1;
}

void EU::genFBO(unsigned int* mFB, int width, int height) {
	glGenFramebuffers(1, mFB);
	glBindFramebuffer(GL_FRAMEBUFFER, *mFB);
}

void EU::genFBODepthmap(unsigned int* handle, int width, int height, unsigned precision) {
	glGenTextures(1, handle);
	glBindTexture(GL_TEXTURE_2D, *handle);
	glTexImage2D(GL_TEXTURE_2D, 0, precision, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *handle, 0);
}

void EU::genFBOTexture(unsigned int* handle, int width, int height, unsigned attachment, int min_f, int max_f) {
	glGenTextures(1, handle);
	glBindTexture(GL_TEXTURE_2D, *handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_f);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, max_f);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, *handle, 0);
}

void EU::genSSAOTexture(unsigned int* handle, int width, int height, unsigned attachment, int min_f, int max_f) {
	glGenTextures(1, handle);
	glBindTexture(GL_TEXTURE_2D, *handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_f);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, max_f);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, *handle, 0);
}

void EU::glUseAttachments(const unsigned* attachments,int size){
	glDrawBuffers(size, attachments);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void EU::render_objects_compute(Engine* e) {
	//for each(Object* o in e->objects) {
	//	float length2 = 0;
	//}
}

void EU::create_objects(Engine* e) {
	//TestModel* tm = new TestModel();
	//e->add_object(tm);
	/*MaxModel* mm = new MaxModel();
	mm->mModelMatrix = mat4(1.0);
	mm->mModelMatrix = translate(mm->mModelMatrix, vec3(0, -3, - 2));
	e->add_object(mm);*/
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
            Engine::check_gl_error("pre max loading");
			MaxModel* mm = new MaxModel();
			mm->mModelMatrix = mat4(1.0);
			mm->mModelMatrix = translate(mm->mModelMatrix, vec3(5*i+2, 0, 3*j+2));
			e->add_object(mm);
		}
	}
}
