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
        InputDispatcher() = default;
        void ProcessInput(const InputData2& input);
        void Dispatch();
    private:
        InputData2 m_Input;
        InputData2 m_Last;
        bool m_ShouldAggregate;
    };
}

#endif