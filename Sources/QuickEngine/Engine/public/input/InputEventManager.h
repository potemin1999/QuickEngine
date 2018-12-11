#pragma once

#include <vector>
#include "input/InputEvent.h"
#include "input/InputReceiver.h"

using namespace std;

namespace QE {

    class InputEventManager {
    private:

        vector<InputEvent *> keyDownEvents, keyUpEvents;
        vector<InputReceiver *> receivers;
        vector<int> pressedKeys;

        int processInputEvent(InputEvent *event);

    public:

        void registerInputReceiver(InputReceiver *receiver);

        void unregisterInputReceiver(InputReceiver *receiver);

        void injectInputEvent(InputEvent *event);

        void processDelayedEvents();

    };

}