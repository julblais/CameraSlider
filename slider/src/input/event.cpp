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

bool Event::operator==(const Event& other) const
{
    return button == other.button &&
        dpadButtonState == other.dpadButtonState &&
        joystickButton == other.joystickButton &&
        joystickButtonState == other.joystickButtonState &&
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
    return dpadButtonState != ButtonNone;
}

bool Event::HasJoystickChange() const
{
    return joystickButtonState != ButtonNone || joystickDirectionChanged;
}

Event Process(const InputData& last, const InputData& current)
{
    Event event;
    if (last.IsDown() && !current.IsDown() ||
        last.IsUp() && !current.IsUp() ||
        last.IsLeft() && !current.IsLeft() ||
        last.IsRight() && !current.IsRight() ||
        last.IsSelect() && !current.IsSelect())
        event.dpadButtonState = ButtonReleased;
    else if (!last.IsDown() && current.IsDown() ||
        !last.IsUp() && current.IsUp() ||
        !last.IsLeft() && current.IsLeft() ||
        !last.IsRight() && current.IsRight() ||
        !last.IsSelect() && current.IsSelect())
        event.dpadButtonState = ButtonPressed;
    else
        event.dpadButtonState = ButtonNone;

    if (last.IsCenterButton() && !current.IsCenterButton())
        event.joystickButtonState = ButtonReleased;
    else if (!last.IsCenterButton() && current.IsCenterButton())
        event.joystickButtonState = ButtonPressed;
    else
        event.joystickButtonState = ButtonNone;

    event.button = current.dpadButton;
    event.joystickX = current.x;
    event.joystickY = current.y;
    event.joystickDirectionChanged = last.x != current.x || last.y != current.y;
    event.joystickButton = current.joystickButton;

    return event;
}

void EventDispatcher::ProcessInput(const InputData& input)
{
    if (m_AggregateCount == 0)
    {
        m_Input = input;
        m_AggregateCount++;
    }
    else
    {   //last input wins
        if (input.DpadActive())
            m_Input.dpadButton = input.dpadButton;
        if (input.IsCenterButton())
            m_Input.joystickButton = input.joystickButton;
        if (input.x != 0 || input.y != 0)
        {
            m_Input.x = input.x;
            m_Input.y = input.y;
        }
    }
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
    auto event = Process(m_Last, m_Input);
    DebugPrint(m_LastEvent, event);
        
    SendEvent(event);
    m_AggregateCount = 0;
    m_Last = m_Input;
    m_Input = InputData();
    m_LastEvent = event;
}