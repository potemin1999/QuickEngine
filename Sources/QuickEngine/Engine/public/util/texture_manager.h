#pragma once

#include "texture.h"

namespace QECore {

    class TextureManager {
    public:

        static Texture loadTexture(char *filename);

    };

}