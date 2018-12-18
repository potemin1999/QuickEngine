#include <cstdio>
#include <input/InputEventManager.h>

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

void InputEventManager::injectKeyInputEvent(QE::KeyInputEvent *event) {
    if (event->keyAction == ACTION_PRESS) {
        // save key down event for triggering
        keyDownEvents.push_back(event);

        pressedKeys.push_back(event->keyCode);

    } else if (event->keyAction == ACTION_RELEASE) {
        // save key up event for triggering
        keyUpEvents.push_back(event);

        for (int i = 0; i < pressedKeys.size(); i++) {
            if (pressedKeys[i] == event->keyCode) {
                pressedKeys.erase(pressedKeys.begin() + i);
            }
        }
    } else if (event->keyAction == ACTION_REPEAT) {
        keyRepeatEvents.push_back(event);
    }
}

void InputEventManager::injectMouseInputEvent(QE::MouseInputEvent *event) {
    if (event->action == ACTION_PRESS) {
        // save key down event for triggering
        mouseDownEvents.push_back(event);

        pressedMouseButtons.push_back(event->button);
    } else if (event->action == ACTION_RELEASE) {
        // save key up event for triggering
        mouseUpEvents.push_back(event);

        for (int i = 0; i < pressedMouseButtons.size(); i++) {
            if (pressedMouseButtons[i] == event->button) {
                pressedMouseButtons.erase(pressedMouseButtons.begin() + i);
            }
        }
    }
}

void InputEventManager::injectCharInputEvent(CharInputEvent *event) {
    charEvents.push_back(event);
}

void InputEventManager::processDelayedEvents() {
    // process keyboard events
    for (auto &event : keyDownEvents) {
        processKeyInputEvent(event);
        delete event;
    }
    keyDownEvents.clear();

    for (auto &event : keyRepeatEvents) {
        processKeyInputEvent(event);
        delete event;
    }
    keyRepeatEvents.clear();

    for (auto &event : charEvents) {
        processCharInputEvent(event);
        delete event;
    }
    charEvents.clear();

    for (int key: pressedKeys) {
        KeyInputEvent event{key, ACTION_PRESSED};
        processKeyInputEvent(&event);
    }

    for (auto &event : keyUpEvents) {
        processKeyInputEvent(event);
        delete event;
    }
    keyUpEvents.clear();


    // process mouse events
    for (auto &event : mouseDownEvents) {
        processMouseInputEvent(event);
        delete event;
    }
    mouseDownEvents.clear();

    for (int key: pressedMouseButtons) {
        MouseInputEvent event{key, ACTION_PRESSED};
        processMouseInputEvent(&event);
    }

    for (auto &event : mouseUpEvents) {
        processMouseInputEvent(event);
        delete event;
    }
    mouseUpEvents.clear();
}

int InputEventManager::processKeyInputEvent(QE::KeyInputEvent *event) {
    for (auto &receiver : receivers) {
        InputProcessResult result = receiver->onKeyInputEvent(event);
    }
}

int InputEventManager::processMouseInputEvent(MouseInputEvent *event) {
    for (auto &receiver : receivers) {
        InputProcessResult result = receiver->onMouseInputEvent(event);
    }
}

void InputEventManager::processCharInputEvent(CharInputEvent *event) {
    for (auto &receiver : receivers) {
        InputProcessResult result = receiver->onCharInputEvent(event);
    }
}
