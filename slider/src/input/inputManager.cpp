#include "inputManager.h"

Input::InputManager::InputManager(const InputData &defaultInput):
    m_LastInput(defaultInput)
{}

void Input::InputManager::ProcessInput(const InputData &input)
{
    auto dpad = input.dpad;
    auto lastDpad = m_LastInput.dpad;

    SendInputEvent(input);
    
    if (lastDpad.IsDown() && !dpad.IsDown())
        SendKeyReleasedEvent(DpadButton::Down);
    else if (lastDpad.IsUp() && !dpad.IsUp())
        SendKeyReleasedEvent(DpadButton::Up);
    else if (lastDpad.IsLeft() && !dpad.IsLeft())
        SendKeyReleasedEvent(DpadButton::Left);
    else if (lastDpad.IsRight() && !dpad.IsRight())
        SendKeyReleasedEvent(DpadButton::Right);
    else if (lastDpad.IsSelect() && !dpad.IsSelect())
        SendKeyReleasedEvent(DpadButton::Select);
    else if (!lastDpad.IsDown() && dpad.IsDown())
        SendKeyPressedEvent(DpadButton::Down);
    else if (!lastDpad.IsUp() && dpad.IsUp())
        SendKeyPressedEvent(DpadButton::Up);
    else if (!lastDpad.IsLeft() && dpad.IsLeft())
        SendKeyPressedEvent(DpadButton::Left);
    else if (!lastDpad.IsRight() && dpad.IsRight())
        SendKeyPressedEvent(DpadButton::Right);
    else if (!lastDpad.IsSelect() && dpad.IsSelect())
        SendKeyPressedEvent(DpadButton::Select);
}

void Input::InputManager::SendInputEvent(const InputData &input)
{
    this->SendEvent([&input](IInputListener* listener){
        listener->OnInput(input);
    });
}

void Input::InputManager::SendKeyPressedEvent(DpadButton button)
{
    this->SendEvent([&button](IInputListener* listener){
        listener->OnButtonPressed(button);
    });
}

void Input::InputManager::SendKeyReleasedEvent(DpadButton button)
{
    this->SendEvent([&button](IInputListener* listener){
        listener->OnButtonReleased(button);
    });
}
