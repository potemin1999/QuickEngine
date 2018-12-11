#pragma once

#include "Defines.h"
#include "GameObject.h"
#include "ModelLoader.h"

class NowhereSign_01 : public GameObject {
public:
    NowhereSign_01() : GameObject() {
        ModelLoader::load_object(this, "", "nowhere.obj");
    };
};