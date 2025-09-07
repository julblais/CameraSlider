#include "buttonEvent.h"

const char* Input::ToString(const ButtonEvent button)
{
    switch (button)
    {
        case DpadUp:
            return "Up";
        case DpadDown:
            return "Down";
        case DpadLeft:
            return "Left";
        case DpadRight:
            return "Right";
        case DpadSelect:
            return "Select";
        case StickCenter:
            return "Center";
        case None:
            return "None";
        default:
            return "Unknown";
    }
}

const char* Input::ToString(const ButtonChange button)
{
    switch (button)
    {
        case ButtonNone:
            return "None";
        case ButtonPressed:
            return "Pressed";
        case ButtonReleased:
            return "Released";
        default:
            return "Unknown";
    }
}