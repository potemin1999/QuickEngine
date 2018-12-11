//
// Created by ilya on 9/24/18.
//

#include "input/input_event_manager.h"
#include "util/log.h"

using namespace QECore;

void InputEventManager::registerInputReceiver(QECore::InputReceiver *receiver) {
    receivers.push_back(receiver);
    log("added input receiver at %p\n", receiver);
}

void InputEventManager::unregisterInputReceiver(QECore::InputReceiver *receiver) {
    for (int i = 0; i < receivers.size(); i++) {
        if (receivers[i] == receiver) {
            receivers.erase(receivers.begin() + i);
        }
    }
}

void InputEventManager::injectInputEvent(QECore::InputEvent *event) {
    events.push_back(event);
}

void InputEventManager::processDelayedEvents() {
    for (int i = 0; i < events.size(); i++) {
        processInputEvent(events[i]);
        delete events[i];
    }
    events.clear();
}

int InputEventManager::processInputEvent(QECore::InputEvent *event) {
    for (int i = 0; i < receivers.size(); i++) {
        InputProcessResult result = receivers[i]->onInputEvent(event);
        /*if (result == INPUT_INTERCEPTED) {
            break;
        }*/
    }
}
