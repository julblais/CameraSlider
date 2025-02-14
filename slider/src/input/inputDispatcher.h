#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "input.h"
#include "src/core/event/eventSource.h"
#include <functional>

namespace Input
{
    class InputDispatcher : public Core::EventSource<const Event&>
    {
    public:
        void ProcessInput(const InputData& input);
    private:
        InputData m_LastInput;
    };
}

#endif