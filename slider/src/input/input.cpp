#include "input.h"

const char* Input::ToString(DpadButton button)
{
    switch (button)
    {
        case DpadLeft:
            return "Left";
            break;
        case DpadRight:
            return "Right";
            break;
        case DpadUp:
            return "Up";
            break;
        case DpadDown:
            return "Down";
            break;
        case DpadSelect:
            return "Select";
            break;
        default:
            return "Unknown";
            break;
    }
}

const char* Input::ToString(JoystickButton button)
{
    switch (button)
    {
        case JoystickCenter:
            return "Center";
            break;
        case JoystickNone:
            return "None";
            break;
        default:
            return "Unknown";
            break;
    }
}

bool Input::Event::HasChange() const
{
    return HadDpadChange() || HasJoystickChange();
}

bool Input::Event::HadDpadChange() const
{
    return dpadButtonState != ButtonNone;
}

bool Input::Event::HasJoystickChange() const
{
    return joystickButton != JoystickNone || joystickDirectionChanged;
}

Input::InputData2::InputData2(
    const DpadButton button, const JoystickButton joystickButton, 
    const float x, const float y)
    : button(button), joystickButton(joystickButton), x(x), y(y)
{}
