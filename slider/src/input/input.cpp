#include "input.h"

const char *Input::DpadInput::ToString(DpadButton button)
{
    switch (button)
    {
        case DpadLeft:
            return "None";
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
/*
const char *Input::DpadInput::ToString(DpadState state)
{
    switch (state)
    {
        case DpadPressed:
            return "Pressed";
            break;
        case DpadRelease:
            return "Released";
            break;
        default:
            return "Unknown";
            break;
    }
}
*/