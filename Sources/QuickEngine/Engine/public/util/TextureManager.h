#pragma once

#include "Texture.h"

namespace QE {

    class TextureManager {
    public:

        static Texture loadTexture(char *filename);

    };

}