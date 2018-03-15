#pragma once

#include "glad\glad.h"
#include "glm\glm.hpp"
#include "engine.h"
#include "material.h"
#include "mesh.h"
using namespace glm;

class Object {
public:
	Object() {
		mModelMatrix = glm::mat4(1.0f);
	};
		//mModelMatrix = mat4(1.0f);
		//material = new Material();
	
	Mesh* meshes;
	int mesh_count;
	mat4 mModelMatrix;
	
};
