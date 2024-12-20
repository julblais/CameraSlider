#include "input.h"

const char* Input::DpadInput::ToString(DpadButton button)
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

const char *Input::JoystickInput::ToString(JoystickButton button)
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
