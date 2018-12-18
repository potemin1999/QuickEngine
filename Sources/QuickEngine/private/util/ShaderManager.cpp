#include "glad/glad.h"
#include "Engine.h"
#include "util/ShaderManager.h"


int ShaderManager::loadVertexShader(char *v) {
    int vertex_handle = glCreateShader(GL_VERTEX_SHADER);
    if (vertex_handle != 0) {
        string s = EU::load_string(v);
        const char *c = s.c_str();
        //printf("\n vertex shader: \n %s \n end of vertex shader \n",c);
        glShaderSource(vertex_handle, 1, &c, nullptr);
        glCompileShader(vertex_handle);
        int *status = new int[1];
        glGetShaderiv(vertex_handle, GL_COMPILE_STATUS, status);
        if (status[0] != 0) {
            log("vertex shader compiled succeed to link %i from file %s \n", vertex_handle, v);
        } else {
            char logC[1024];
            int *l = 0;
            glGetShaderInfoLog(vertex_handle, 1024, l, logC);
            log("vertex shader compile failed : %s\n", logC);
            glDeleteShader(vertex_handle);
            delete l;
        }
        delete status;
    } else {
        log("vertex handle NULL");
    }
    return vertex_handle;
}

int ShaderManager::loadFragmentShader(char *f) {
    int fragment_handle = glCreateShader(GL_FRAGMENT_SHADER);
    if (fragment_handle != 0) {
        string s = EU::load_string(f);
        const char *c = s.c_str();
        //printf("%s", c);
        glShaderSource(fragment_handle, 1, &c, nullptr);
        glCompileShader(fragment_handle);
        int *status = new int[1];
        glGetShaderiv(fragment_handle, GL_COMPILE_STATUS, status);
        if (status[0] != 0) {
            log("fragment shader compiled succeed to link %i from file %s \n", fragment_handle, f);
        } else {
            char logC[1024];
            int *l = 0;
            glGetShaderInfoLog(fragment_handle, 1024, l, logC);
            log("fragment shader compile failed : %s\n", logC);
            glDeleteShader(fragment_handle);
        }
    } else {
        log("fragment handle NULL");
    }
    return fragment_handle;
}


int ShaderManager::link_shaders(int program_handle, int v_handle, int f_handle) {
    if (program_handle != 0) {
        glLinkProgram(program_handle);
        int *status = new int[1];
        glGetProgramiv(program_handle, GL_LINK_STATUS, status);
        if (status[0] != 0) {
            log("shader program linked succeed to link %i\n", program_handle);
            glUseProgram(program_handle);
        } else {
            char logC[1024];
            int *l = 0;
            glGetProgramInfoLog(program_handle, 1024, l, logC);
            log("shader program linking failed : %s\n", logC);
            glDeleteProgram(program_handle);
        }
        glReleaseShaderCompiler();
    } else {
        log("program handle NULL");
    }
    return program_handle;
}

int ShaderManager::load_shaders(char *v, char *f) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        log("before program creting error %i: \n", error);
    }
    int program_handle = glCreateProgram();
    if (program_handle != 0) {
        int vertex_handle = loadVertexShader(v);
        int fragment_shader = loadFragmentShader(f);
        glAttachShader(program_handle, vertex_handle);
        glAttachShader(program_handle, fragment_shader);
        glLinkProgram(program_handle);
        int *status = new int[1];
        glGetProgramiv(program_handle, GL_LINK_STATUS, status);
        if (status[0] != 0) {
            log("shader program linked succeed to link %i\n", program_handle);
            glUseProgram(program_handle);
        } else {
            char logC[1024];
            int *l = 0;
            glGetProgramInfoLog(program_handle, 1024, l, logC);
            log("shader program linking failed : %s\n", logC);
            glDeleteProgram(program_handle);
        }
        glReleaseShaderCompiler();
    } else {
        log("program handle NULL");
    }

    return program_handle;
}

int ShaderManager::use_shader_program(int program) {
    glUseProgram(program);
    return 0;
}

int ShaderManager::use_shader_program(Shader *shader) {
    glUseProgram(shader->getProgram());
    return 0;
}

Shader *ShaderManager::create_shader_program(char *vertex_path, char *fragment_path) {
    Shader *shader = new Shader(vertex_path, fragment_path);
    shader->compile();
    return shader;
}

void ShaderManager::get_uniform(Shader *s, char *name, unsigned int *ref) {
    log("get uniform %u %s %u\n", s->getProgram(), name, *ref);
    *ref = glGetUniformLocation(s->getProgram(), name);
}

int ShaderManager::get_uniform(Shader *s, char *name) {
    return glGetUniformLocation(s->getProgram(), name);
}