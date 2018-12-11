#pragma once

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