#pragma once

#include "qe/core/graphics/defines.h"
#include "qe/core/settings.h"
#include "qe/core/engine.h"

class TextureManager {
public:
    static Texture loadTexture(char *filename);

    static int useTexture(Texture *texture);

    static int freeTexture(Texture *texture);
};