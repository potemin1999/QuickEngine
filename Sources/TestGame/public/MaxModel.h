#pragma once

#include "Defines.h"
#include "GameObject.h"
#include "ModelLoader.h"

class MaxModel : public GameObject {
public:
    MaxModel(World *world) : GameObject(world) {
        this->model = ModelLoader::load_object("characters/max/", "max_caulfield.obj");
    };
};