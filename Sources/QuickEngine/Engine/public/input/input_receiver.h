//
// Created by ilya on 9/24/18.
//

#ifndef QUICKENGINE_INPUT_RECEIVER_H
#define QUICKENGINE_INPUT_RECEIVER_H

#include "input_event.h"
#include "input_processing_result.h"

namespace QECore {

    class InputReceiver {
    public:
        
        virtual void onReceiverRegistered() {}

        virtual void onReceiverUnregistered() {}

        virtual InputProcessResult onInputEvent(InputEvent *event) = 0;
        
    };

}

#endif //QUICKENGINE_INPUT_RECEIVER_H
