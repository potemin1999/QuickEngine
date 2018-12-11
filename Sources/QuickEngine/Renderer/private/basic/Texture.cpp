//
// Created by ilya on 9/26/18.
//

#include <glad/glad.h>
#include "Texture.h"

#ifdef DEBUG_TEXTURE
#define DEBUG_TEXTURE_OUTPUT
#include <iostream>
#endif // DEBUG_TEXTURE

#include "util/Log.h"

using namespace QE;

Texture::Texture() = default;

Texture::Texture(int _format, int _internalFormat) {
    min_filter = GL_NEAREST;
    mag_filter = GL_NEAREST;
    format = _format;
    internalFormat = _internalFormat;
    gen(&pointer);
    bindTexture();
}

Texture::Texture(int _width, int _height, int _format, int _internalFormat) {
    format = _format;
    internalFormat = _internalFormat;
    gen(&pointer);
    bindTexture();
    setSize(_width, _height);
}

Texture::Texture(int _width, int _height, int _format, int _internalFormat, void *_data, int _data_type) {
    format = _format;
    internalFormat = _internalFormat;
    gen(&pointer);
    bindTexture();
    setSize(_width, _height);
    setData(_data, _data_type);
}

Texture::~Texture() = default;

void Texture::setData(void *_data, int _data_type) {
    bindTexture();
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, internalFormat, _data_type, _data);
    log("creating texture with w:%d , h:%d\n", width, height);
    loaded = true;
}

void Texture::setSize(int _width, int _height) {
    width = _width;
    height = _height;
}

void Texture::setFilters(int _min_filter, int _mag_filter) {
    bindTexture();
    min_filter = _min_filter;
    mag_filter = _mag_filter;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
}

void Texture::gen(unsigned int *pointer) {
    glGenTextures(1, pointer);
    log("generated new texture with pointer = %u\n", *pointer);
}

int Texture::bindTexture() {
    //if (current==this) return 1;
    glBindTexture(GL_TEXTURE_2D, pointer);
    //current = this;
    return 0;
}

void Texture::use() {
    bindTexture();
}

bool Texture::isLoaded() {
    return loaded;
}

int Texture::free() {
    if (!pointer) return -1;
    glDeleteTextures(1, &pointer);
    return 0;
}