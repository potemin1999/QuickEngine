#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "qe/core/graphics/materials/material_manager.h"
#include "mesh.h"
using namespace glm;

class Object {
public:
	Object() {
		mModelMatrix = glm::mat4(1.0f);
	};

    ~Object() {
        mesh_count = 0;
        if (meshes)
            delete meshes;
    }
		//mModelMatrix = mat4(1.0f);
		//material = new Material();
	
	Mesh* meshes;
    unsigned long mesh_count;
	mat4 mModelMatrix;
	
};
