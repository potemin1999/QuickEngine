//
// Created by ilya on 9/24/18.
//
#ifndef QUICKENGINE_INPUT_EVENT_MANAGER_H
#define QUICKENGINE_INPUT_EVENT_MANAGER_H

#include <vector>
#include "input_event.h"
#include "input_receiver.h"

using namespace std;

namespace QEInput {

    class InputEventManager {
    private:
        vector<InputReceiver *> receivers;
    public:
        void registerInputReceiver(InputReceiver *receiver);

        void unregisterInputReceiver(InputReceiver *receiver);

        int processInputEvent(InputEvent *event);
    };

    class InputEventManagerProvider {
    public:
        virtual InputEventManager *getInputEventManager();
    };
}
#endif //QUICKENGINE_INPUT_EVENT_MANAGER_H
