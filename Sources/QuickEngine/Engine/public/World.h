//
// Created by ilya on 9/24/18.
//

#ifndef QUICKENGINE_WORLD_H
#define QUICKENGINE_WORLD_H

#include "Camera.h"

using namespace QE;

namespace QE {

    class World {
    private:

        Camera *currentCamera;

    public:

        World() {}

        ~World() {}

        Camera *getCurrentCamera() {
            return currentCamera;
        }

        virtual void onCreate();

        virtual void onDestroy();

    };

}

#endif //QUICKENGINE_WORLD_H
