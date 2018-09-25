#define ENGINE_UTILS_INIT_SUCCEED 0x00000000

#include "qe/core/graphics/defines.h"
#include "qe/core/util/string_helper.h"
//#include <direct.h>
#include "engine.h"
#include "max_model.h"

using namespace QECore;

int EU::init_utils(Engine* e) {
    e->checkGlError("eu pre init");
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
        delete f_max;
    }
    e->checkGlError("eu init");
	return ENGINE_UTILS_INIT_SUCCEED;
}



string EU::load_string(char* filename) {
	char path[1024];
    getcwd(path, sizeof(path));
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
	log("read file %s\n", _path.data());
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
    //Engine::checkGlError("pre gen vbo and vao");
	glGenBuffers(1, &vbo1);
    //Engine::checkGlError("gen vbo and vao");
	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    //Engine::checkGlError("bind vbo and vao");
	glBufferData(GL_ARRAY_BUFFER, size, arr, GL_STATIC_DRAW);
    //Engine::checkGlError("bind data vbo and vao");
	unsigned int vao1;
	glGenVertexArrays(1, &vao1);
	glBindVertexArray(vao1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float)*3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float)*5));
	glBindVertexArray(0);
	log("created vbo and vao with size %i, array on address %p\n",size,&verts);
    Engine::checkGlError("get vbo and vao");
	verts = vao1;
}

void EU::create_objects(Engine* e) {
	//TestModel* tm = new TestModel();
    //e->addObject(tm);
    /*MaxModel* mm = new MaxModel();
    mm->mModelMatrix = mat4(1.0);
    mm->mModelMatrix = translate(mm->mModelMatrix, vec3(0, -3, - 2));
    e->addObject(mm);*/
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 1; j++) {
            Engine::checkGlError("pre max loading");
			MaxModel* mm = new MaxModel();
			mm->mModelMatrix = mat4(1.0);
			mm->mModelMatrix = translate(mm->mModelMatrix, vec3(5*i+2, 0, 3*j+2));
            e->addObject(mm);
		}
	}
}
