//
// Created by Ilya on 3/15/2018.
//

#ifndef QUICKENGINE_SHADER_H
#define QUICKENGINE_SHADER_H

namespace QECore {

    class Shader {

    private:

        unsigned int program = 0;
        unsigned int vertex = 0;
        unsigned int fragment = 0;
        char *vertex_source;
        char *fragment_source;

        void *uniforms;

        int createShader(unsigned int *handle, const char *source, int shader_type);

        int createProgram(unsigned int *program, const unsigned *vertex, const unsigned *fragment);


    public:

        Shader(const char *_vertex_source, const char *_fragment_source);

        virtual ~Shader();

        virtual int compile();

        virtual int use();

        virtual int freeCache();

        unsigned getProgram() {
            return program;
        }

        unsigned uniform(const char *name);

        unsigned getUniform(const char *name);

        int uniformMatrix4fv(const char *name, int count, bool transpose, const float *pointer);

        int uniform3fv(const char *name, int count, const float *pointer);

        int uniform2f(const char *name, float f1, float f2);

        int uniform1f(const char *name, float f1);

        int uniform1i(const char *name, int i1);

    };

}
#endif //QUICKENGINE_SHADER_H
