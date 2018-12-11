#pragma once

#include "gameobject.h"
#include "model_loader.h"
#include "engine.h"
#include <glm/glm.hpp>

class TestModel : public GameObject {
public:
    TestModel() : GameObject() {
//		mat.tex = 0;

        //ModelLoader::load_OBJ("models\\floating_crystal.obj", meshes->data, meshes->vert_count);

        //ModelLoader::load_object(this, "","max_caulfield.obj");
        //for (int i = 0; i < mesh_count; i++) {
        //	printf("    mesh[%i] name %s\n", i, (meshes + i)->name);
        //}

        ModelLoader::load_object(this, "characters/lara/", "Lara_Croft_v1.obj");
        for (int i = 0; i < mesh_count; i++) {
            printf("    mesh[%i] name %s\n", i, (meshes + i)->name);
        }

        mModelMatrix = mat4(1.0);
        glm::translate(mModelMatrix, vec3(100.0f, 0, 100.0f));

        //meshes->material = MaterialManager::create_material("textures\\grass_a.png", "textures\\river_rock_n.png");

        /*
        meshes = new Mesh();
        meshes->name = "main mesh";
        //ModelLoader::load_OBJ("models\\max_caulfield.obj", meshes->data, meshes->vert_count);
        ModelLoader::load_OBJ("models\\floating_crystal.obj", meshes->data, meshes->vert_count);
        //meshes->material = MaterialManager::create_material("textures\\max_suit.dds", "textures\\river_rock_n.png");
        meshes->material = new Material;
        meshes->material = MaterialManager::create_material("textures\\grass_a.png", "textures\\river_rock_n.png");
        mesh_count = 1;*/
    };
};
