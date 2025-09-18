#include "eventDispatcher.h"
#include "core/utils/enumUtils.h"

using namespace IO;
using namespace Core::Enums;

InputData Merge(const InputData& a, const InputData& b)
{
    const auto buttons = a.buttons | b.buttons;
    const auto x  = a.x != 0 ? a.x : b.x;
    const auto y  = a.y != 0 ? a.y : b.y;
    return InputData(buttons, x, y);
}

EventDispatcher::EventDispatcher() :
    m_AggregateCount(0) {}

void EventDispatcher::ProcessInput(const InputData& input)
{
    if (m_AggregateCount == 0)
    {
        m_Input = input;
        m_AggregateCount++;
    }
    else
        m_Input = Merge(m_Input, input);
}

void EventDispatcher::Dispatch()
{
    const auto event = Event(m_Last, m_Input);
    event.Log();
    SendEvent(event);

    m_AggregateCount = 0;
    m_Last = m_Input;
    m_Input = InputData();
}
