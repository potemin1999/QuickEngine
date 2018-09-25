//
// Created by ilya on 9/24/18.
//

#ifndef QUICKENGINE_WORLD_H
#define QUICKENGINE_WORLD_H

#include <qe/core/graphics/basic/camera.h>

using namespace QEGraphics;

namespace QECore {

    class World {
    private:
        Camera *currentCamera;
    public:
        Camera *getCurrentCamera() {
            return currentCamera;
        }

        virtual void onCreate();

        virtual void onDestroy();
    };

}

#endif //QUICKENGINE_WORLD_H
