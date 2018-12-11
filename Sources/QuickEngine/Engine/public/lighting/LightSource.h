//
// Created by Ilya on 3/16/2018.
//

#ifndef QUICKENGINE_LIGHT_SOURCE_H
#define QUICKENGINE_LIGHT_SOURCE_H

#include <glm/glm.hpp>

using namespace glm;

namespace QE {

    class LightSource {
    private:
        vec3 *color;
    public:
        LightSource() {}
    };
}
#endif //QUICKENGINE_LIGHT_SOURCE_H
