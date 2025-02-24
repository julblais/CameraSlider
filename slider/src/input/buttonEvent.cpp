#include "buttonEvent.h"

const char* Input::ToString(ButtonEvent button)
{
    switch (button)
    {
        case DpadUp:
            return "Up";
            break;
        case DpadDown:
            return "Down";
            break;
        case DpadLeft:
            return "Left";
            break;
        case DpadRight:
            return "Right";
            break;
        case DpadSelect:
            return "Select";
            break;
        case StickCenter:
            return "Center";
            break;
        case DpadNone:
            return "None";
            break;
        default:
            return "Unknown";
            break;
    }
}

const char* Input::ToString(ButtonChange button)
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