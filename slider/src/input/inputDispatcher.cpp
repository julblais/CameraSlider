#include "inputDispatcher.h"

void SetButtonState(Input::DpadButton button, Input::ButtonState state, Input::Event& out)
{
    out.button = button;
    out.dpadButtonState = state;
}

void SetButtonState(Input::JoystickButton button, Input::ButtonState state, Input::Event& out)
{
    out.joystickButton = button;
    out.joystickButtonState = state;
}

void Process(const Input::DpadInput& last, const Input::DpadInput& current, Input::Event& out)
{
    if (last.IsDown() && !current.IsDown())
        SetButtonState(Input::DpadDown, Input::ButtonReleased, out);
    else if (last.IsUp() && !current.IsUp())
        SetButtonState(Input::DpadUp, Input::ButtonReleased, out);
    else if (last.IsLeft() && !current.IsLeft())
        SetButtonState(Input::DpadLeft, Input::ButtonReleased, out);
    else if (last.IsRight() && !current.IsRight())
        SetButtonState(Input::DpadRight, Input::ButtonReleased, out);
    else if (last.IsSelect() && !current.IsSelect())
        SetButtonState(Input::DpadSelect, Input::ButtonReleased, out);
    else if (!last.IsDown() && current.IsDown())
        SetButtonState(Input::DpadDown, Input::ButtonPressed, out);
    else if (!last.IsUp() && current.IsUp())
        SetButtonState(Input::DpadUp, Input::ButtonPressed, out);
    else if (!last.IsLeft() && current.IsLeft())
        SetButtonState(Input::DpadLeft, Input::ButtonPressed, out);
    else if (!last.IsRight() && current.IsRight())
        SetButtonState(Input::DpadRight, Input::ButtonPressed, out);
    else if (!last.IsSelect() && current.IsSelect())
        SetButtonState(Input::DpadSelect, Input::ButtonPressed, out);
}

void Process(const Input::JoystickInput& last, const Input::JoystickInput& current, Input::Event& out)
{
    if (last.IsCenterButton() && !current.IsCenterButton())
        SetButtonState(Input::JoystickCenter, Input::ButtonReleased, out);
    else if (!last.IsCenterButton() && current.IsCenterButton())
        SetButtonState(Input::JoystickCenter, Input::ButtonPressed, out);

    out.joystickX = current.x;
    out.joystickY = current.y;
}

void Input::InputDispatcher::ProcessInput(const InputData &input)
{
    auto event = Event();
    Process(m_LastInput.dpad, input.dpad, event);
    Process(m_LastInput.joystick, input.joystick, event);
    
    SendEventWithCapture([event](IInputListener* listener) -> bool {
        return listener->OnInputEvent(event);
    });

    m_LastInput = input;
}