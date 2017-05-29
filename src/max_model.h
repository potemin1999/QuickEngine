#pragma once

#include "object.h"
#include "model_loader.h"
#include "engine.h"

class MaxModel : public Object {
public:
	MaxModel() : Object() {
		//		mat.tex = 0;

		//ML::load_OBJ("models\\floating_crystal.obj", meshes->data, meshes->vert_count);

		ML::load_object(this, "characters\\max\\", "max_caulfield.obj");
		for (int i = 0; i < mesh_count; i++) {
			printf("    mesh[%i] name %s\n", i, (meshes + i)->name);
		}

		//meshes->material = MH::create_material("textures\\grass_a.png", "textures\\river_rock_n.png");

		/*
		meshes = new Mesh();
		meshes->name = "main mesh";
		ML::load_OBJ("models\\max_caulfield.obj", meshes->data, meshes->vert_count);
		meshes->material = MH::create_material("textures\\max_suit.dds", "textures\\river_rock_n.png");
		mesh_count = 1;*/
	};
};