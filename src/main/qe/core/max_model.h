#pragma once

#include "qe/core/graphics/defines.h"
#include "object.h"
#include "model_loader.h"
#include "engine.h"

class MaxModel : public Object {
public:
	MaxModel() : Object() {
		//		mat.tex = 0;

        //ModelLoader::load_OBJ("models\\floating_crystal.obj", meshes->data, meshes->vert_count);

        ModelLoader::load_object(this, "characters/max/", "max_caulfield.obj");

#ifdef GRAPHICs_DEBUG
		for (int i = 0; i < mesh_count; i++) {
			printf("    mesh[%i] name %s\n", i, (meshes + i)->name);
		}
#endif

        //meshes->material = MaterialManager::create_material("textures\\grass_a.png", "textures\\river_rock_n.png");

        /*
        meshes = new Mesh();
        meshes->name = "main mesh";
        ModelLoader::load_OBJ("models\\max_caulfield.obj", meshes->data, meshes->vert_count);
        meshes->material = MaterialManager::create_material("textures\\max_suit.dds", "textures\\river_rock_n.png");
        mesh_count = 1;*/
	};
};