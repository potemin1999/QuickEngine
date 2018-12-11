#pragma once

#include "GameObject.h"
#include <fstream>
#include <list>
#include <fstream>
#include <vector>

#ifdef GRAPHICS_DEBUG
#include <iostream>
#endif

#include <sstream>
#include <vector>
#include <cstring>
#include "util/StringHelper.h"

using namespace QE;

namespace QE {

    class ModelLoader {

    public:
        static void load_object(GameObject *container, const char *path, const char *name);

        static void load_object(GameObject *container, char *path, char *name);

        static void load_OBJ(char *path, int &vao, int &vertex_count);


    };

}
