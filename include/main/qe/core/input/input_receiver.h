//
// Created by ilya on 9/24/18.
//

#ifndef QUICKENGINE_INPUT_RECEIVER_H
#define QUICKENGINE_INPUT_RECEIVER_H

#include <qe/core/input/input_event.h>
#include <qe/core/input/input_processing_result.h>

namespace QECore {

    class InputReceiver {
    public:
        
        virtual void onReceiverRegistered() {}

        virtual void onReceiverUnregistered() {}

        virtual InputProcessResult onInputEvent(InputEvent *event) = 0;
        
    };

}

#endif //QUICKENGINE_INPUT_RECEIVER_H
