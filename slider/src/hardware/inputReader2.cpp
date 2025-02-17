#include "inputReader2.h"
#include "src/debug.h"
#include "src/core/utils/mathUtils.h"

using namespace Hardware;

constexpr auto JOYSTICK_RANGE_X = 4096;
constexpr auto JOYSTICK_RANGE_Y = 4096;

InputReader::InputReader(const InputPins& pins)
    : m_Pins(pins)
{}

void InputReader::Setup()
{
    pinMode(m_Pins.dpadUp, INPUT_PULLUP);
    pinMode(m_Pins.dpadDown, INPUT_PULLUP);
    pinMode(m_Pins.dpadLeft, INPUT_PULLUP);
    pinMode(m_Pins.dpadRight, INPUT_PULLUP);
    pinMode(m_Pins.dpadSelection, INPUT_PULLUP);
    pinMode(m_Pins.joystickVertical, INPUT);
    pinMode(m_Pins.joystickHorizontal, INPUT);
    pinMode(m_Pins.joystickCenter, INPUT_PULLUP);
}

InputData2 Hardware::InputReader::ReadInput()
{
    auto up = digitalRead(m_Pins.dpadUp) == LOW;
    auto down = digitalRead(m_Pins.dpadDown) == LOW;
    auto left = digitalRead(m_Pins.dpadLeft) == LOW;
    auto right = digitalRead(m_Pins.dpadRight) == LOW;
    auto selection = digitalRead(m_Pins.dpadSelection) == LOW;
    auto v = analogRead(m_Pins.joystickVertical);
    auto h = analogRead(m_Pins.joystickHorizontal);
    auto joystickSelection = digitalRead(m_Pins.joystickCenter) == LOW;

    auto dpadButton = Input::DpadNone;
    if (left)
        dpadButton = Input::DpadLeft;
    else if (right)
        dpadButton = Input::DpadRight;
    else if (up)
        dpadButton = Input::DpadUp;
    else if (down)
        dpadButton = Input::DpadDown;
    else if (selection)
        dpadButton = Input::DpadSelect;

    auto x = -Core::Remap((float)h, 0.0f, (float)JOYSTICK_RANGE_X, -1.0f, 1.0f);
    auto y = Core::Remap((float)v, 0.0f, (float)JOYSTICK_RANGE_Y, -1.0f, 1.0f);
    auto joystickButton = joystickSelection ? Input::JoystickCenter : Input::JoystickNone;

    return InputData2(dpadButton, joystickButton, x, y);
}