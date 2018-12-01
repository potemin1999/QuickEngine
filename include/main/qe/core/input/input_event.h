//
// Created by ilya on 9/24/18.
//

#ifndef QUICKENGINE_INPUT_EVENT_H
#define QUICKENGINE_INPUT_EVENT_H

#include <qe/core/input/input_key.h>

namespace QECore{

    struct InputEvent {
        int keyCode;
        int keyAction;
    };

}
#endif //QUICKENGINE_INPUT_EVENT_H
