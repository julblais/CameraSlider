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
    : dpadEvent(input.dpadButton),
    stickEvent(input.joystickButton),
    joystickX(input.x),
    joystickY(input.y)
{
    diff = CreateDiff(previous, input);
}

bool Event::operator==(const Event& other) const
{
    return dpadEvent == other.dpadEvent &&
        stickEvent == other.stickEvent &&
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
    if (event.HasDpadChange())
    {
        LogDebug("\tdpad\t", Input::ToString(event.GetDpadChange()),
            "\t", Input::ToString(event.GetDpadChange()));
    }
    if (event.HasStickChange())
    {
        LogDebug("\tjoystick\t", Input::ToString(event.GetStickChange()),
            "\t", Input::ToString(event.GetStickChange()), "\t", event.GetStickX(), "\t", event.GetStickY());
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

const char* Input::ToString(ButtonChange button)
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

const char* Input::ToString(DpadEvent button)
{
    switch (button)
    {
        case DpadUp:
            return "Up";
            break;
        case DpadDown:
            return "Down";
            break;
        case DpadLeft:
            return "Left";
            break;
        case DpadRight:
            return "Right";
            break;
        case DpadSelect:
            return "Select";
            break;
        case DpadNone:
            return "None";
            break;
        default:
            return "Unknown";
            break;
    }
}

const char* Input::ToString(StickEvent button)
{
    switch (button)
    {
        case StickCenter:
            return "Center";
            break;
        case StickNone:
            return "None";
            break;
        default:
            return "Unknown";
            break;
    }
}
