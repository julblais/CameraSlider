#include "input.h"

Input::Event::Event():
    button(DpadNone),
    dpadButtonState(ButtonNone), 
    joystickButton(JoystickNone), 
    joystickButtonState(ButtonNone), 
    joystickX(0), 
    joystickY(0)
{}

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