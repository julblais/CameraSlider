#include "event.h"
#include "src/debug.h"
#include "src/core/utils/enumUtils.h"

using namespace Input;
using namespace Core::Enums;

Event::Diff::Diff(ButtonEvent pressed, ButtonEvent released, bool stickMoved)
    : released(released), pressed(pressed), stickMoved(stickMoved), change(ButtonChange::None)
{
    if (pressed != DpadNone)
        change |= ButtonChange::Pressed;
    if (released != DpadNone)
        change |= ButtonChange::Released;
}

Event::Diff Event::CreateDiff(const Event& previous, const InputData& input)
{
    ButtonEvent pressed = DpadNone;
    ButtonEvent released = DpadNone;

    if (previous.IsDpadDown() && !input.IsDown())
        released = DpadDown;
    else if (previous.IsDpadUp() && !input.IsUp())
        released = DpadUp;
    else if (previous.IsDpadLeft() && !input.IsLeft())
        released = DpadLeft;
    else if (previous.IsDpadRight() && !input.IsRight())
        released = DpadRight;
    else if (previous.IsDpadSelect() && !input.IsSelect())
        released = DpadSelect;
    else if (!previous.IsDpadDown() && input.IsDown())
        pressed = DpadDown;
    else if (!previous.IsDpadUp() && input.IsUp())
        pressed = DpadUp;
    else if (!previous.IsDpadLeft() && input.IsLeft())
        pressed = DpadLeft;
    else if (!previous.IsDpadRight() && input.IsRight())
        pressed = DpadRight;
    else if (!previous.IsDpadSelect() && input.IsSelect())
        pressed = DpadSelect;
    else if (previous.IsStickCenter() && !input.IsCenterButton())
        released = StickCenter;
    else if (!previous.IsStickCenter() && input.IsCenterButton())
        pressed = StickCenter;

    bool stickMoved = previous.GetStickX() != input.x || previous.GetStickY() != input.y;
    return Diff(pressed, released, stickMoved);
}

Event::Event(const Event& previous, const InputData& input)
    : button(input.button),
    joystickX(input.x),
    joystickY(input.y)
{
    diff = CreateDiff(previous, input);
}

void Merge(const InputData& input, InputData& destination)
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
    if (event.HasButtonChange())
    {
        LogDebug("\tEvent button:\t", Input::ToString(event.GetButtonEvent()),
            "\tpressed: ", Input::ToString(event.GetButtonPressed()),
            "\treleased: ", Input::ToString(event.GetButtonReleased()));
    }
    if (event.HasStickMoved())
    {
        LogDebug("\tEvent joystick\t", event.GetStickX(), "\t", event.GetStickY());
    }
};
#else
void DebugPrint(const Event& lastEvent, const Event& event) {}
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
