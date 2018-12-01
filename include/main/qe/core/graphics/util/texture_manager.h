#pragma once

#include <qe/core/graphics/basic/texture.h>

namespace QECore {

    class TextureManager {
    public:

        static Texture loadTexture(char *filename);

    };

}