#pragma once

#include "GameObject.h"

class Player : public GameObject {
public:
    bool isInSecondWorld = false;

    explicit Player(World *world);

    World *portableWorld;

    void moveToWorld(World *world);
};

