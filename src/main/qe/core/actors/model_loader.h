#pragma once

#include "qe/core/graphics/defines.h"
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
#include "qe/core/engine.h"
#include "qe/core/util/string_helper.h"

using namespace QECore;

namespace QECore {

    class ModelLoader {

    public:
        static void load_object(Object *container, const char *path, const char *name);

        static void load_object(Object *container, char *path, char *name);

        static void load_OBJ(char *path, int &vao, int &vertex_count);


    };

}
