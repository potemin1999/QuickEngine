#pragma once

#include "input/InputEventManager.h"

using namespace QE;

namespace QE {

    class EngineContext {
    public:

        EngineContext() = default;

        virtual ~EngineContext() = default;

        virtual InputEventManager *getInputEventManager() = 0;

    };
}