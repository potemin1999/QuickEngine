//
// Created by ilya on 9/24/18.
//
#ifndef QUICKENGINE_INPUT_EVENT_MANAGER_H
#define QUICKENGINE_INPUT_EVENT_MANAGER_H

#include <vector>
#include "input/input_event.h"
#include "input/input_receiver.h"

using namespace std;

namespace QECore {

    class InputEventManager {
    private:

        vector<InputEvent*> events;
        vector<InputReceiver *> receivers;

        int processInputEvent(InputEvent *event);

    public:

        void registerInputReceiver(InputReceiver *receiver);

        void unregisterInputReceiver(InputReceiver *receiver);

        void injectInputEvent(InputEvent* event);

        void processDelayedEvents();

    };

}
#endif //QUICKENGINE_INPUT_EVENT_MANAGER_H
