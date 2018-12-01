#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "mesh.h"

using namespace glm;

namespace QECore {

    class Object {
    public:

        Object();

        ~Object();

        Mesh *meshes;
        unsigned long mesh_count;
        mat4 mModelMatrix;

        void *getModelMatrix();

    };

}
