#pragma once

#include "GameObject.h"

class Player : public GameObject {
public:
    bool isInSecondWorld = false;

    Player(World *world);

    World *portableWorld;

    void moveToWorld(World *world);
};

