//
// Created by ilya on 9/24/18.
//

#include <qe/core/graphics/renderer.h>

using namespace QEGraphics;

RendererInputReceiver::RendererInputReceiver(Renderer *renderer) {
    parent = renderer;
}

InputProcessResult RendererInputReceiver::onInputEvent(InputEvent *event) {
    printf("received input action %d with keycode %d\n", event->keyAction, event->keyCode);
}
