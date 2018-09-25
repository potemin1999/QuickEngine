//
// Created by Ilya on 3/16/2018.
//

#ifndef QUICKENGINE_DIRECTIONAL_LIGHT_SOURCE_H
#define QUICKENGINE_DIRECTIONAL_LIGHT_SOURCE_H

#include "graphics/defines.h"
#include "light_source.h"

namespace QEGraphics {
    class DirectionalLightSource : LightSource {
    private:
        vec3 *direction;

    };
}
#endif //QUICKENGINE_DIRECTIONAL_LIGHT_SOURCE_H
