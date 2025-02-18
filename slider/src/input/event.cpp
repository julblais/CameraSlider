#include "event.h"
#include "src/debug.h"

using namespace Input;

const char* Input::ToString(ButtonState button)
{
    switch (button)
    {
        case ButtonNone:
            return "None";
            break;
        case ButtonPressed:
            return "Pressed";
            break;
        case ButtonReleased:
            return "Released";
            break;
        default:
            return "Unknown";
            break;
    }
}

EventDiff CreateDiff(const Event& previous, const InputData& input)
{
    EventDiff diff;
    ButtonState dpadState = ButtonNone;
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

    ButtonState joystickState = ButtonNone;
    if (last.IsCenterButton() && !current.IsCenterButton())
        joystickState = ButtonReleased;
    else if (!last.IsCenterButton() && current.IsCenterButton())
        joystickState = ButtonPressed;

}

Event::Event(const Event& previous, const InputData& input)
    : button(input.dpadButton),
    joystickButton(input.joystickButton),
    joystickX(input.x),
    joystickY(input.y)
{
    diff = CreateDiff(previous, input);
}

bool Event::operator==(const Event& other) const
{
    return button == other.button &&
        joystickButton == other.joystickButton &&
        joystickX == other.joystickX &&
        joystickY == other.joystickY;
}

bool Event::operator!=(const Event& other) const
{
    return !(*this == other);
}

bool Event::HasChange() const
{
    return HadDpadChange() || HasJoystickChange();
}

bool Event::HadDpadChange() const
{
    return diff.dpadState != ButtonNone;
}

bool Event::HasJoystickChange() const
{
    return diff.joystickState != ButtonNone || diff.joystickDirectionChanged;
}

InputData Merge(const InputData& input, InputData& destination)
{
    //last input wins
    if (input.DpadActive())
        destination.dpadButton = input.dpadButton;
    if (input.IsCenterButton())
        destination.joystickButton = input.joystickButton;
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
    if (event.HadDpadChange())
    {
        LogDebug("\tdpad\t", Input::ToString(event.button),
            "\t", Input::ToString(event.dpadButtonState));
    }
    if (event.HasJoystickChange())
    {
        LogDebug("\tjoystick\t", Input::ToString(event.joystickButton),
            "\t", Input::ToString(event.joystickButtonState), "\t", event.joystickX, "\t", event.joystickY);
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