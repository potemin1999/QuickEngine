#pragma once

#include <vector>
#include "input/input_event.h"
#include "input/input_receiver.h"

using namespace std;

namespace QECore {

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