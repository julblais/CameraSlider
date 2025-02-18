#include "deviceInputReader.h"
#include "src/debug.h"
#include "src/core/utils/mathUtils.h"

using namespace Hardware;

constexpr auto JOYSTICK_RANGE_X = 4096;
constexpr auto JOYSTICK_RANGE_Y = 4096;

DeviceInputReader::DeviceInputReader(const InputPins& pins)
    : m_Pins(pins)
{}

void DeviceInputReader::Setup()
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

Input::InputData DeviceInputReader::ReadInput()
{
    auto up = digitalRead(m_Pins.dpadUp) == LOW;
    auto down = digitalRead(m_Pins.dpadDown) == LOW;
    auto left = digitalRead(m_Pins.dpadLeft) == LOW;
    auto right = digitalRead(m_Pins.dpadRight) == LOW;
    auto selection = digitalRead(m_Pins.dpadSelection) == LOW;
    auto v = analogRead(m_Pins.joystickVertical);
    auto h = analogRead(m_Pins.joystickHorizontal);
    auto joystickSelection = digitalRead(m_Pins.joystickCenter) == LOW;

    auto dpadButton = Input::DpadButton::None;
    if (left)
        dpadButton = Input::DpadButton::Left;
    else if (right)
        dpadButton = Input::DpadButton::Right;
    else if (up)
        dpadButton = Input::DpadButton::Up;
    else if (down)
        dpadButton = Input::DpadButton::Down;
    else if (selection)
        dpadButton = Input::DpadButton::Select;

    auto x = -Core::Remap((float)h, 0.0f, (float)JOYSTICK_RANGE_X, -1.0f, 1.0f);
    auto y = Core::Remap((float)v, 0.0f, (float)JOYSTICK_RANGE_Y, -1.0f, 1.0f);
    auto joystickButton = joystickSelection ? Input::JoystickButton::Center : Input::JoystickButton::None;

    return Input::InputData(dpadButton, joystickButton, x, y);
}