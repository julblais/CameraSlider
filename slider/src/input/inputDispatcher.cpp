#include "inputDispatcher.h"

Input::InputDispatcher::InputDispatcher(const InputData &defaultInput):
    m_LastInput(defaultInput)
{}

void Input::InputDispatcher::ProcessInput(const InputData &input)
{
    auto dpad = input.dpad;
    auto lastDpad = m_LastInput.dpad;
    
    if (lastDpad.IsDown() && !dpad.IsDown())
        SendButtonReleasedEvent(DpadButton::Down);
    else if (lastDpad.IsUp() && !dpad.IsUp())
        SendButtonReleasedEvent(DpadButton::Up);
    else if (lastDpad.IsLeft() && !dpad.IsLeft())
        SendButtonReleasedEvent(DpadButton::Left);
    else if (lastDpad.IsRight() && !dpad.IsRight())
        SendButtonReleasedEvent(DpadButton::Right);
    else if (lastDpad.IsSelect() && !dpad.IsSelect())
        SendButtonReleasedEvent(DpadButton::Select);
    else if (!lastDpad.IsDown() && dpad.IsDown())
        SendButtonPressEvent(DpadButton::Down);
    else if (!lastDpad.IsUp() && dpad.IsUp())
        SendButtonPressEvent(DpadButton::Up);
    else if (!lastDpad.IsLeft() && dpad.IsLeft())
        SendButtonPressEvent(DpadButton::Left);
    else if (!lastDpad.IsRight() && dpad.IsRight())
        SendButtonPressEvent(DpadButton::Right);
    else if (!lastDpad.IsSelect() && dpad.IsSelect())
        SendButtonPressEvent(DpadButton::Select);

    m_LastInput = input;
}

void Input::InputDispatcher::SendButtonPressEvent(DpadButton button)
{
    this->SendEvent([&button](IInputListener* listener){
        listener->OnButtonPressed(button);
    });
}

void Input::InputDispatcher::SendButtonReleasedEvent(DpadButton button)
{
    this->SendEvent([&button](IInputListener* listener){
        listener->OnButtonReleased(button);
    });
}
