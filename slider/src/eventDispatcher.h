#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include "core/eventSource.h"
#include "event.h"

namespace IO
{
    class EventDispatcher : public Core::EventSource<const Event&>
    {
    public:
        EventDispatcher();
        void ProcessInput(const InputData& input);
        void Dispatch();

    private:
        InputData m_Input;
        InputData m_Last;
        unsigned int m_AggregateCount;
    };
}

#endif
