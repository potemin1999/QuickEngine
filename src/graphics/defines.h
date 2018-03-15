#pragma once

//
// Created by Ilya on 3/15/2018.
//


#ifndef QUICKENGINE_GRAPHICS_DEFINES_H
#define QUICKENGINE_GRAPHICS_DEFINES_H

#define GRAPHICS_LOG

#ifdef DEBUG
#define GRAPHICS_DEBUG
#define DEBUG_TEXTURE
#define DEBUG_SURFACE
#endif // DEBUG

#ifdef GRAPHICS_LOG
#include <iostream>
#endif

#if ( defined(_WIN32) | defined(__WIN32) | defined(__linux__) )

#include "glad/glad.h"
#include "khr/khrplatform.h"

#elif ( defined(_ANDROID_) )

#endif

namespace QEGraphics{
    class Texture;
    class Framebuffer;
    class Shader;
}

inline void log(const char* format, ...){
#ifdef GRAPHICS_LOG
    va_list args;
    va_start(args,format);
    /*const char* prefix = "[GRAPHICS]:";
    char* newFormat = new char[strlen(prefix)+strlen(format)];
    strcpy(newFormat,prefix);
    strcpy(newFormat+strlen(prefix),format);*/
    vprintf(format,args);
    va_end(args);
#endif
}

#endif //QUICKENGINE_DEFINES_H
