#define ENGINE_UTILS_INIT_SUCCEED 0x00000000

#include "defines.h"
#include "util/string_helper.h"
#include "engine.h"
#include "settings.h"
#include "../../../TestGame/public/max_model.h"
#include "../../../TestGame/public/floor_model.h"
#include <unistd.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace QECore;

int EU::init_utils(Engine *e) {
    QECore::Engine::checkGlError("eu pre init");
    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 1);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (anisotropy > 1) {
        auto f_max = new float;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, f_max);
        delete f_max;
    }
    Engine::checkGlError("eu init");
    return ENGINE_UTILS_INIT_SUCCEED;
}


string EU::load_string(char *filename) {
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
    std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
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
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *) (sizeof(float) * 3));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *) (sizeof(float) * 5));
    glBindVertexArray(0);
    log("created vbo and vao with size %i, array on address %p\n", size, &verts);
    Engine::checkGlError("get vbo and vao");
    verts = vao1;
}

void EU::create_objects(Engine *e) {

}
