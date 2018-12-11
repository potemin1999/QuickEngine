#pragma once


#define LOG

#ifdef DEBUG
#define GRAPHICS_DEBUG
#define DEBUG_TEXTURE
#define DEBUG_SURFACE
#endif // DEBUG

#ifdef LOG

#include <iostream>

#endif

#if (defined(_WIN32) | defined(__WIN32) | defined(__linux__))

#elif (defined(_ANDROID_))

#endif

namespace QECore {
    class Texture;

    class Framebuffer;

    class Shader;
}

#ifdef LOG

#include <stdarg.h>

#endif
