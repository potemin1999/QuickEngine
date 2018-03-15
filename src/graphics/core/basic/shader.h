//
// Created by Ilya on 3/15/2018.
//

#ifndef QUICKENGINE_SHADER_H
#define QUICKENGINE_SHADER_H

#include "graphics/defines.h"
#include <map>

namespace QEGraphics{

    class Shader{

    private:

        //static Shader* current = 0;
        unsigned int program;
        unsigned int vertex;
        char* vertex_source;
        unsigned int fragment;
        char* fragment_source;
        std::map<const char*,unsigned int> *uniforms;

        int createShader(unsigned int* handle,const char* source,int shader_type){
            *handle = glCreateShader(shader_type);
            if (*handle){
                glShaderSource(*handle,1,&source, nullptr);
                //log("uploaded shader : \n %s \n",source);
                glCompileShader(*handle);
                int* status = new int[1];
                glGetShaderiv(*handle,GL_COMPILE_STATUS,status);
                if (status[0]!=0) {
                    log("shader program linked succeed to link %i\n", *handle);
                }else{
                    char logC[1024];
                    glGetShaderInfoLog(*handle, 1024, 0, logC);
                    log("shader compile failed : %s\n",logC);
                    glDeleteShader(*handle);
                }
            }else{
                log("shader handle == 0\n");
            }
            return 0;
        }

        int createProgram(unsigned int* program, unsigned int* vertex, unsigned int* fragment){
            *program = glCreateProgram();
            if (*program){
                glAttachShader(*program,*vertex);
                glAttachShader(*program,*fragment);
                glLinkProgram(*program);
                int* status = new int[1];
                glGetProgramiv(*program,GL_LINK_STATUS,status);
                if (status[0]!=0) {
                    log("program linked succeed to link %i\n", *program);
                }else{
                    char logC[1024];
                    glGetProgramInfoLog(*program, 1024, 0, logC);
                    log("shader program linking failed : %s\n",logC);
                    glDeleteProgram(*program);
                }
                glReleaseShaderCompiler();
            }else{
                log("program handle == 0\n");
            }
            return 0;
        }

        string loadSource(char* filename) {
            char path[1024];
            _getcwd(path, sizeof(path));
            string _path = string(path);
            _path.append("\\");
            _path.append(filename);
            ifstream file(_path);
            if (!file.is_open()) {
                log("Error loading file %s\n", _path.data());
                return "";
            }
            std::string str((std::istreambuf_iterator<char>(file)),   std::istreambuf_iterator<char>());
            file.close();
            return str;
        }

    public:


        Shader(const char* _vertex_source,const char* _fragment_source){
            //log("creating new shader : vertex : %s and fragment %s",_vertex_source,_fragment_source);
            vertex_source = new char[strlen(_vertex_source)];
            fragment_source = new char[strlen(_fragment_source)];
            strcpy(vertex_source,_vertex_source);
            strcpy(fragment_source,_fragment_source);
            uniforms = new std::map<const char*,unsigned int>();
        }

        ~Shader(){
            freeCache();
        }

        int compile(){
            //log("compiling shader\n");
            //log("creating new shader : vertex : %s and fragment %s",vertex_source,fragment_source);
            string vertex_source_loaded = loadSource(vertex_source);
            string fragment_source_loaded = loadSource(fragment_source);
            //log("creating new shader : vertex text : %s and fragment text %s",vertex_source_loaded,fragment_source_loaded);
            createShader(&vertex, vertex_source_loaded.c_str(),GL_VERTEX_SHADER);
            createShader(&fragment,fragment_source_loaded.c_str(),GL_FRAGMENT_SHADER);
            createProgram(&program,&vertex,&fragment);
            return 0;
        }

        int use(){
            //if (current==this) return 1;
            glUseProgram(program);
            //current = this;
            return 0;
        }

        int freeCache(){
            if (fragment_source) delete fragment_source;
            if (vertex_source) delete vertex_source;
            return 0;
        }

        unsigned int getProgram(){
            return program;
        }

        unsigned int uniform(const char* name){
            unsigned int uni = 0;
            uni = glGetUniformLocation(program,name);
            (*uniforms)[name] = uni;
            return uni;
        }

        unsigned int getUniform(const char* name){
            return (*uniforms)[name];
        }


        int uniformMatrix4fv(const char* name,int count,bool transpose,const float* pointer){
            unsigned int uniform = getUniform(name);
            glUniformMatrix4fv(uniform,count,transpose,pointer);
            return uniform;
        }

        int uniform3fv(const char* name,int count,const float* pointer){
            unsigned int uniform = getUniform(name);
            glUniform3fv(uniform,count,pointer);
            return uniform;
        }

        int uniform2f(const char* name,float f1,float f2){
            unsigned int uniform = getUniform(name);
            glUniform2f(uniform,f1,f2);
            return uniform;
        }

        int uniform1f(const char* name,float f1){
            unsigned int uniform = getUniform(name);
            glUniform1f(uniform,f1);
            return uniform;
        }

        int uniform1i(const char* name,int i1){
            unsigned int uniform = getUniform(name);
            glUniform1i(uniform,i1);
            return uniform;
        }

    };

}
#endif //QUICKENGINE_SHADER_H
