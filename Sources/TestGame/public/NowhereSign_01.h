#pragma once

#include "Defines.h"
#include "GameObject.h"
#include "ModelLoader.h"

class NowhereSign_01 : public GameObject {
public:
    NowhereSign_01(World *world) : GameObject(world) {
        this->model = ModelLoader::load_object("", "nowhere.obj");
    };
};