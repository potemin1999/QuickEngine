//
// Created by ilya on 9/24/18.
//

#include "input_event_manager.h"

using namespace QEInput;

void InputEventManager::registerInputReceiver(QEInput::InputReceiver *receiver) {
    receivers.push_back(receiver);
}

void InputEventManager::unregisterInputReceiver(QEInput::InputReceiver *receiver) {
    for (int i = 0; i < receivers.size(); i++) {
        if (receivers[i] == receiver) {
            receivers.erase(receivers.begin() + i);
        }
    }
}

int InputEventManager::processInputEvent(QEInput::InputEvent *event) {
    for (int i = 0; i < receivers.size(); i++) {
        InputProcessResult result = receivers[i]->onInputEvent(event);
        if (result == INPUT_INTERCEPTED) {
            break;
        }
    }
}
