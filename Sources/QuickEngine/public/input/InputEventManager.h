#pragma once

#include <vector>
#include "input/InputReceiver.h"

using namespace std;

namespace QE {

    class InputEventManager {
    private:

        vector<KeyInputEvent *> keyDownEvents, keyUpEvents, keyRepeatEvents;
        vector<MouseInputEvent *> mouseDownEvents, mouseUpEvents;
        vector<CharInputEvent *> charEvents;
        vector<InputReceiver *> receivers;
        vector<int> pressedKeys, pressedMouseButtons;

        int processKeyInputEvent(KeyInputEvent *event);

        int processMouseInputEvent(MouseInputEvent *event);

    public:

        void registerInputReceiver(InputReceiver *receiver);

        void unregisterInputReceiver(InputReceiver *receiver);

        void injectKeyInputEvent(KeyInputEvent *event);

        void injectMouseInputEvent(MouseInputEvent *event);

        void injectCharInputEvent(CharInputEvent *event);

        void processDelayedEvents();

        void processCharInputEvent(CharInputEvent *event);
    };

}