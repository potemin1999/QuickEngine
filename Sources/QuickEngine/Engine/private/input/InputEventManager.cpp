//
// Created by ilya on 9/24/18.
//

#include <cstdio>
#include "input/InputEventManager.h"
#include "util/Log.h"

using namespace QE;

void InputEventManager::registerInputReceiver(QE::InputReceiver *receiver) {
    receivers.push_back(receiver);
    receiver->onReceiverRegistered();
    log("added input receiver\n");
}

void InputEventManager::unregisterInputReceiver(QE::InputReceiver *receiver) {
    for (int i = 0; i < receivers.size(); i++) {
        if (receivers[i] == receiver) {
            receivers[i]->onReceiverUnregistered();
            receivers.erase(receivers.begin() + i);
            log("removed input receiver\n");
        }
    }
}

void InputEventManager::injectInputEvent(QE::InputEvent *event) {
    if (event->keyAction == ACTION_DOWN) {
        // save key down event for triggering
        keyDownEvents.push_back(event);

        pressedKeys.push_back(event->keyCode);

    } else if (event->keyAction == ACTION_UP) {
        // save key up event for triggering
        keyUpEvents.push_back(event);

        for (int i = 0; i < pressedKeys.size(); i++) {
            if (pressedKeys[i] == event->keyCode) {
                pressedKeys.erase(pressedKeys.begin() + i);
            }
        }
    }
}

void InputEventManager::processDelayedEvents() {
    for (auto &event : keyDownEvents) {
        processInputEvent(event);
        delete event;
    }
    keyDownEvents.clear();

    for (int key: pressedKeys) {
        InputEvent event{key, ACTION_PRESSED};
        processInputEvent(&event);
    }

    for (auto &event : keyUpEvents) {
        processInputEvent(event);
        delete event;
    }
    keyUpEvents.clear();
}

int InputEventManager::processInputEvent(QE::InputEvent *event) {
    for (auto &receiver : receivers) {
        InputProcessResult result = receiver->onInputEvent(event);
    }
}
