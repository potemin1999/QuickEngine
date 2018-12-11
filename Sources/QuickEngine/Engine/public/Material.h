//
// Created by ilya on 9/24/18.
//

#ifndef QUICKENGINE_MATERIAL_H
#define QUICKENGINE_MATERIAL_H

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

#endif //QUICKENGINE_MATERIAL_H
