//
// Created by Ilya on 3/15/2018.
//

#ifndef QUICKENGINE_TEXTURE_H
#define QUICKENGINE_TEXTURE_H

namespace QECore {

    class Texture {
    private:

        bool loaded = false;
        unsigned pointer = 0;
        int format = -1;
        int internalFormat = -1;

        int min_filter = 0;
        int mag_filter = 0;

        int bindTexture();

        void gen(unsigned int *pointer);

    public:

        int id;
        char *source;
        int width = -1;
        int height = -1;

        Texture();

        Texture(int _format, int _internalFormat);

        Texture(int _width, int _height, int _format, int _internalFormat);

        Texture(int _width, int _height, int _format, int _internalFormat, void *_data, int _data_type);

        ~Texture();

        void setData(void *_data, int _data_type);

        void setSize(int _width, int _height);

        void setFilters(int _min_filter, int _mag_filter);

        void use();

        unsigned getPointer() {
            return pointer;
        }

        bool isLoaded();

        int free();

    };

}

#endif //QUICKENGINE_TEXTURE_H
