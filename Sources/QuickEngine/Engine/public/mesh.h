#pragma once

#include "cstring"
#include "material.h"

namespace QECore {

    struct Mesh {
        Material *material = nullptr;
        int vert_count = 0;
        float xyz[3] = {0, 0, 0};
        int data = 0;
        char *name = (char *) "unnamed";
    };

}