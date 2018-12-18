#pragma once

#include "GameObject.h"
#include <fstream>
#include <list>
#include <vector>
#include "Model.h"
#include <sstream>
#include <cstring>
#include "util/StringHelper.h"

#ifdef GRAPHICS_DEBUG
#include <iostream>
#endif

using namespace QE;

namespace QE {
    extern std::map<std::string, Model *> loadedModels;

    class ModelLoader {

    public:
        static void load_object(GameObject *container, const char *path, const char *name);

        static void load_object(GameObject *container, char *path, char *name);

        static void load_OBJ(char *path, int &vao, int &vertex_count);


    };

}
