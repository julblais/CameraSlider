#include "event.h"
#include "src/debug.h"

using namespace Input;

EventDiff CreateDiff(const Event& previous, const InputData& input)
{
    EventDiff diff;
    ButtonChange dpadState = ButtonNone;
    if (last.IsDown() && !current.IsDown() ||
        last.IsUp() && !current.IsUp() ||
        last.IsLeft() && !current.IsLeft() ||
        last.IsRight() && !current.IsRight() ||
        last.IsSelect() && !current.IsSelect())
        dpadState = ButtonReleased;
    else if (!last.IsDown() && current.IsDown() ||
        !last.IsUp() && current.IsUp() ||
        !last.IsLeft() && current.IsLeft() ||
        !last.IsRight() && current.IsRight() ||
        !last.IsSelect() && current.IsSelect())
        dpadState = ButtonPressed;

    ButtonChange joystickState = ButtonNone;
    if (last.IsCenterButton() && !current.IsCenterButton())
        joystickState = ButtonReleased;
    else if (!last.IsCenterButton() && current.IsCenterButton())
        joystickState = ButtonPressed;

}

Event::Event(const Event& previous, const InputData& input)
    : button(input.button),
    joystickX(input.x),
    joystickY(input.y)
{
    diff = CreateDiff(previous, input);
}

bool Event::operator==(const Event& other) const
{
    return button == other.button &&
        joystickX == other.joystickX &&
        joystickY == other.joystickY;
}

bool Event::operator!=(const Event& other) const
{
    return !(*this == other);
}

InputData Merge(const InputData& input, InputData& destination)
{
    //last input wins
    if (input.IsActive())
        destination.button = input.button;
    if (input.x != 0 || input.y != 0)
    {
        destination.x = input.x;
        destination.y = input.y;
    }
}

void EventDispatcher::ProcessInput(const InputData& input)
{
    if (m_AggregateCount == 0)
    {
        m_Input = input;
        m_AggregateCount++;
    }
    else
        Merge(input, m_Input);
}

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
void DebugPrint(const Event& lastEvent, const Event& event)
{
    if (lastEvent == event)
        return;

    LogDebug("Event:");
    if (event.HasButtonChange())
    {
        LogDebug("\tbutton\t", Input::ToString(event.GetButtonEvent()),
            "\t", Input::ToString(event.GetButtonChange()));
    }
    if (event.HasStickMoved())
    {
        LogDebug("\tjoystick\t", event.GetStickX(), "\t", event.GetStickY());
    }
};
#elif
void DebugPrint(const Event& event) {}
#endif

void EventDispatcher::Dispatch()
{
    auto event = Event(m_LastEvent, m_Input);
    DebugPrint(m_LastEvent, event);

    SendEvent(event);
    m_AggregateCount = 0;
    m_Input = InputData();
    m_LastEvent = event;
}