#include "event.h"
#include "src/debug.h"

using namespace Input;

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

Event Process(const InputData2& last, const InputData2& current)
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
        event.dpadButtonState = ButtonNone;

    event.button = current.button;
    event.joystickX = current.x;
    event.joystickY = current.y;
    event.joystickDirectionChanged = last.x != current.x || last.y != current.y;
    event.joystickButton = current.joystickButton;

    return event;
}

void EventDispatcher::ProcessInput(const InputData2& input)
{
    if (!m_ShouldAggregate)
    {
        m_Input = input;
        m_ShouldAggregate = true;
    }
    else
    {   //last input wins
        if (input.DpadActive())
            m_Input.button = input.button;
        if (input.IsCenterButton())
            m_Input.joystickButton = input.joystickButton;
        if (input.x != 0 || input.y != 0)
        {
            m_Input.x = input.x;
            m_Input.y = input.y;
        }
    }
}

void EventDispatcher::Dispatch()
{
    auto event = Process(m_Last, m_Input);
    SendEvent(event);
    m_ShouldAggregate = false;
    m_Last = m_Input;
    m_Input = InputData2();
    if (event.HasChange())
        LogInfo(event);
}