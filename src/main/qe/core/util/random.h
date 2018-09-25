//
// Created by ilya on 9/24/18.
//

#ifndef QUICKENGINE_RANDOM_H
#define QUICKENGINE_RANDOM_H

#include <cstdlib>

float random(float min, float max) {
    float r = (rand() * 1.0f) / (RAND_MAX * 1.0f);
    return min + (max - min) * r;
}

#endif //QUICKENGINE_RANDOM_H
