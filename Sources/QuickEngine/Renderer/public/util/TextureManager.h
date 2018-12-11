#pragma once

#include "Texture.h"
#include <map>
#include <string>

namespace QE {
    extern std::map<std::string, Texture *> loadedTextures;

    class TextureManager {
    public:

        static Texture *loadTexture(std::string path);

    };

}