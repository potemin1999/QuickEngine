#pragma once

#include "InputEvent.h"
#include "InputProcessingResult.h"

namespace QE {

    class InputReceiver {
    public:

        virtual void onReceiverRegistered() {}

        virtual void onReceiverUnregistered() {}

        virtual InputProcessResult onInputEvent(InputEvent *event) = 0;

    };

}