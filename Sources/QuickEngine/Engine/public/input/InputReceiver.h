#pragma once

#include "KeyInputEvent.h"
#include "MouseInputEvent.h"
#include "CharInputEvent.h"
#include "InputProcessingResult.h"

namespace QE {

    class InputReceiver {
    public:

        virtual void onReceiverRegistered() {}

        virtual void onReceiverUnregistered() {}

        virtual InputProcessResult onKeyInputEvent(KeyInputEvent *event) = 0;

        virtual InputProcessResult onMouseInputEvent(MouseInputEvent *event) = 0;

        virtual InputProcessResult onCharInputEvent(CharInputEvent *event) = 0;

    };

}