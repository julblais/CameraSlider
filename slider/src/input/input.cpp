#include "input.h"

using namespace Input;

const char* Input::ToString(DpadButton button)
{
    switch (button)
    {
        case DpadButton::Left :
            return "Left";
            break;
        case DpadButton::Right:
            return "Right";
            break;
        case DpadButton::Up:
            return "Up";
            break;
        case DpadButton::Down:
            return "Down";
            break;
        case DpadButton::Select:
            return "Select";
            break;
        case DpadButton::None:
            return "None";
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
        case JoystickButton::Center:
            return "Center";
            break;
        case JoystickButton::None:
            return "None";
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
