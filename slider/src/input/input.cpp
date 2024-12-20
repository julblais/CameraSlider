#include "input.h"

const char *Input::ToString(DpadButton button)
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

const char *Input::ToString(JoystickButton button)
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

bool Input::Event::HasEvent() const
{
    return button != DpadNone || 
        joystickButton != JoystickNone || 
        joystickX != 0 || 
        joystickY != 0 || 
        dpadButtonState != ButtonNone || 
        joystickButtonState != ButtonNone;
}

bool Input::Event::HasDpadEvent() const
{
    return button != DpadNone;
}

bool Input::Event::HasJoystickEvent() const
{
    return joystickButton != JoystickNone || joystickX != 0 || joystickY != 0;
}
