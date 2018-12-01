#pragma once

//
// Created by Ilya on 3/15/2018.
//


#ifndef QUICKENGINE_GRAPHICS_DEFINES_H
#define QUICKENGINE_GRAPHICS_DEFINES_H

#define LOG

#ifdef DEBUG
#define GRAPHICS_DEBUG
#define DEBUG_TEXTURE
#define DEBUG_SURFACE
#endif // DEBUG

#ifdef LOG
#include <iostream>
#endif

#if ( defined(_WIN32) | defined(__WIN32) | defined(__linux__) )

#include "glad/glad.h"
#include "khr/khrplatform.h"

#elif ( defined(_ANDROID_) )

#endif

namespace QECore{
    class Texture;
    class Framebuffer;
    class Shader;
}

#ifdef LOG

#include <stdarg.h>
#endif


#endif //QUICKENGINE_DEFINES_H
