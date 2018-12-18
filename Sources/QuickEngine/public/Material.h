#pragma once

#include "Texture.h"

using namespace QE;

namespace QE {

    class Material {
    public:

        char *name;
        Texture *t_Ambient;
        Texture *t_Normal;

    };

}