//
// Created by Ilya on 3/15/2018.
//

#ifndef QUICKENGINE_TEXTURE_H
#define QUICKENGINE_TEXTURE_H

#include <qe/core/graphics/defines.h>
#ifdef DEBUG_TEXTURE
#define DEBUG_TEXTURE_OUTPUT
#include <iostream>
#endif // DEBUG_TEXTURE

namespace QEGraphics{

    class Texture{

    private:
        //static Texture* current;

        Framebuffer* attached;
        bool loaded = false;
        unsigned pointer = 0;
        int format = -1;
        int internalformat = -1;

        int min_filter = GL_NEAREST;
        int mag_filter = GL_NEAREST;



        inline int bindTexture(){
            //if (current==this) return 1;
            glBindTexture(GL_TEXTURE_2D, pointer);
            //current = this;
            return 0;
        }

        inline void gen(unsigned int* pointer){
            glGenTextures(1,pointer);
            log("generated new texture with pointer = %u\n",*pointer);
        }

    public:


        int id;
        char* source;

        int width = -1;
        int height = -1;

        Texture(){
            //creating container
        }

        Texture(int _format,int _internalformat){
            format = _format;
            internalformat = _internalformat;
            gen(&pointer);
            bindTexture();
        }

        Texture(int _width,int _height,int _format, int _internalformat){
            format = _format;
            internalformat = _internalformat;
            gen(&pointer);
            bindTexture();
            setSize(_width,_height);
        }

        /*Texture(int _width,int _height){
            format = GL_RGBA8;
            internalformat = GL_RGBA;
            gen(&pointer);
            bindTexture();
            setSize(_width,_height);
        }*/

        Texture(int _width,int _height,int _format,int _internalformat,void* _data,int _data_type){
            format = _format;
            internalformat = _internalformat;
            gen(&pointer);
            bindTexture();
            setSize(_width,_height);
            setData(_data,_data_type);
        }



        ~Texture(){
        }

        void setData(void* _data,int _data_type){
            bindTexture();
            glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,internalformat,_data_type,_data);
            loaded = true;
        }

        void setSize(int _width,int _height){
            width = _width;
            height = _height;
        }

        void setFilters(int _min_filter,int _mag_filter){
            bindTexture();
            min_filter = _min_filter;
            mag_filter = _mag_filter;
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
        }

        void use(){
            bindTexture();
        }

        unsigned getPointer(){
            return pointer;
        }

        bool isLoaded(){
            return loaded;
        }

        void onAttach(Framebuffer* f){
            attached = f;
        }

        int free() {
            if (!pointer) return -1;
            glDeleteTextures(1, &pointer);
            return 0;
        }

    };

}

#endif //QUICKENGINE_TEXTURE_H
