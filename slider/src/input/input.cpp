#include "input.h"

using namespace Input;

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

InputData::InputData(
    DpadButton button, JoystickButton joystickButton, float x, float y)
    : dpadButton(button), joystickButton(joystickButton), x(x), y(y)
{}
