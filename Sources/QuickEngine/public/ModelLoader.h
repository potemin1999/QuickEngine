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
        static Model *load_object(const char *path, const char *name);

        static Model *load_object(char *path, char *name);

        static void load_OBJ(char *path, int &vao, int &vertex_count);


    };

}
