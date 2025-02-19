#include "event.h"
#include "src/debug.h"

using namespace Input;

inline void SetButtonState(ButtonEvent button, ButtonState state, EventDiff& out)
{
    out.button = button;
    out.dpadButtonState = state;
}

//two different buttons can be pressed and released in the same event
//whoops

EventDiff CreateDiff(const Event& previous, const InputData& input)
{
    EventDiff diff;
    if (previous.IsDpadDown() && !current.IsDown())
        SetButtonState(Input::DpadDown, Input::ButtonReleased, out);
    else if (previous.IsUp() && !current.IsUp())
        SetButtonState(Input::DpadUp, Input::ButtonReleased, out);
    else if (previous.IsLeft() && !current.IsLeft())
        SetButtonState(Input::DpadLeft, Input::ButtonReleased, out);
    else if (previous.IsRight() && !current.IsRight())
        SetButtonState(Input::DpadRight, Input::ButtonReleased, out);
    else if (previous.IsSelect() && !current.IsSelect())
        SetButtonState(Input::DpadSelect, Input::ButtonReleased, out);
    else if (previous.IsCenterButton() && !current.IsCenterButton())
        SetButtonState(Input::StickCenter, Input::ButtonReleased, out);
    else if (!previous.IsDown() && current.IsDown())
        SetButtonState(Input::DpadDown, Input::ButtonPressed, out);
    else if (!previous.IsUp() && current.IsUp())
        SetButtonState(Input::DpadUp, Input::ButtonPressed, out);
    else if (!previous.IsLeft() && current.IsLeft())
        SetButtonState(Input::DpadLeft, Input::ButtonPressed, out);
    else if (!previous.IsRight() && current.IsRight())
        SetButtonState(Input::DpadRight, Input::ButtonPressed, out);
    else if (!previous.IsSelect() && current.IsSelect())
        SetButtonState(Input::DpadSelect, Input::ButtonPressed, out);
    else if (!previous.IsCenterButton() && current.IsCenterButton())
        SetButtonState(Input::StickCenter, Input::ButtonPressed, out);

    diff.stickMoved = last.x != current.x || last.y != current.y;
    return diff;
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