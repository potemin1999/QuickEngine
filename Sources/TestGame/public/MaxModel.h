#pragma once

#include "Defines.h"
#include "GameObject.h"
#include "ModelLoader.h"

class MaxModel : public GameObject {
public:
    MaxModel(World *world) : GameObject(world) {
        ModelLoader::load_object(this, "characters/max/", "max_caulfield.obj");
    };
};