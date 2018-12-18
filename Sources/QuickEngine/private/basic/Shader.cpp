//
// Created by ilya on 9/26/18.
//

#include "Shader.h"
#include "Defines.h"
#include "util/Log.h"
#include "util/Utils.h"
#include <fstream>
#include <map>
#include <unistd.h>
#include <cstring>
#include "OpenGL.h"

using namespace QE;

namespace QE {
    namespace ShaderImpl {
        std::string loadSource(char *filename) {
            char path[1024];
            QE::getEngineRootDir(path, sizeof(char) * 1024);
            std::string _path = std::string(path);
            _path.append("/");
            _path.append(filename);
            std::ifstream file(_path, std::ios::binary);
            if (!file.is_open()) {
                log("Error loading file %s\n", _path.data());
                return "";
            }
            std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            file.close();
            return str;
        }
    }
}

Shader::Shader(const char *_vertex_source, const char *_fragment_source) {
    //log("creating new shader : vertex : %s and fragment %s",_vertex_source,_fragment_source);
    vertex_source = new char[std::strlen(_vertex_source)];
    fragment_source = new char[std::strlen(_fragment_source)];
    std::strcpy(vertex_source, _vertex_source);
    std::strcpy(fragment_source, _fragment_source);
    uniforms = new std::map<const char *, unsigned int>();
}

Shader::~Shader() {
    freeCache();
    ((std::map<const char *, unsigned int> *) uniforms)->clear();
    delete (std::map<const char *, unsigned int> *) uniforms;
}

int Shader::compile() {
    //log("compiling shader\n");
    //log("creating new shader : vertex : %s and fragment %s",vertex_source,fragment_source);
    std::string vertex_source_loaded = ShaderImpl::loadSource(vertex_source);
    std::string fragment_source_loaded = ShaderImpl::loadSource(fragment_source);
    //log("creating new shader : vertex text : %s and fragment text %s",vertex_source_loaded,fragment_source_loaded);
    createShader(&vertex, vertex_source_loaded.c_str(), GL_VERTEX_SHADER);
    createShader(&fragment, fragment_source_loaded.c_str(), GL_FRAGMENT_SHADER);
    createProgram(&program, &vertex, &fragment);
    return 0;
}

int Shader::use() {
    glUseProgram(program);
    return 0;
}

int Shader::freeCache() {
    delete fragment_source;
    delete vertex_source;
    return 0;
}

int Shader::createShader(unsigned int *handle, const char *source, int shader_type) {
    *handle = glCreateShader((GLenum) shader_type);
    if (*handle) {
        glShaderSource(*handle, 1, &source, nullptr);
        //log("uploaded shader : \n %s \n",source);
        glCompileShader(*handle);
        auto status = new int[1];
        glGetShaderiv(*handle, GL_COMPILE_STATUS, status);
        if (status[0] != 0) {
            log("shader program linked succeed to link %i\n", *handle);
        } else {
            char logC[1024];
            glGetShaderInfoLog(*handle, 1024, nullptr, logC);
            log("shader compile failed : %s\n", logC);
            glDeleteShader(*handle);
        }
        delete[] status;
    } else {
        log("shader handle == 0\n");
    }
    return 0;
}

int Shader::createProgram(unsigned int *program, const unsigned *vertex, const unsigned *fragment) {
    *program = glCreateProgram();
    if (*program) {
        glAttachShader(*program, *vertex);
        glAttachShader(*program, *fragment);
        glLinkProgram(*program);
        auto status = new int[1];
        glGetProgramiv(*program, GL_LINK_STATUS, status);
        if (status[0] != 0) {
            log("program linked succeed to link %i\n", *program);
        } else {
            char logC[1024];
            glGetProgramInfoLog(*program, 1024, nullptr, logC);
            log("shader program linking failed : %s\n", logC);
            glDeleteProgram(*program);
        }
        glReleaseShaderCompiler();
        delete[] status;
    } else {
        log("program handle == 0\n");
    }
    return 0;
}

unsigned Shader::uniform(const char *name) {
    unsigned uni = 0;
    uni = (unsigned) glGetUniformLocation(program, name);
    (*(std::map<const char *, unsigned int> *) uniforms)[name] = uni;
    return uni;
}

unsigned Shader::getUniform(const char *name) {
    unsigned uni = (*(std::map<const char *, unsigned int> *) uniforms)[name];
    if (!uni) {
        uni = uniform(name);
    }
    return uni;
}

int Shader::uniformMatrix4fv(const char *name, int count, bool transpose, const float *pointer) {
    unsigned uniform = getUniform(name);
    glUniformMatrix4fv(uniform, count, (GLboolean) transpose, pointer);
    return uniform;
}

int Shader::uniform3fv(const char *name, int count, const float *pointer) {
    unsigned int uniform = getUniform(name);
    glUniform3fv(uniform, count, pointer);
    return uniform;
}

int Shader::uniform2f(const char *name, float f1, float f2) {
    unsigned int uniform = getUniform(name);
    glUniform2f(uniform, f1, f2);
    return uniform;
}

int Shader::uniform1f(const char *name, float f1) {
    unsigned int uniform = getUniform(name);
    glUniform1f(uniform, f1);
    return uniform;
}

int Shader::uniform1i(const char *name, int i1) {
    unsigned int uniform = getUniform(name);
    glUniform1i(uniform, i1);
    return uniform;
}
