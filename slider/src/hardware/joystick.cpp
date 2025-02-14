#include "joystick.h"
#include "src/core/utils/mathUtils.h"

#include <esp32-hal-gpio.h>

constexpr auto JOYSTICK_RANGE_X = 4096;
constexpr auto JOYSTICK_RANGE_Y = 4096;

Hardware::Joystick::Joystick(int horizontalPin, int verticalPin, int centerPin) :
    m_HorizontalPin(horizontalPin),
    m_VerticalPin(verticalPin),
    m_CenterPin(centerPin)
{}

void Hardware::Joystick::Init()
{
    pinMode(m_VerticalPin, INPUT);
    pinMode(m_HorizontalPin, INPUT);
    pinMode(m_CenterPin, INPUT_PULLUP);
}

Input::JoystickInput Hardware::Joystick::ReadInput()
{
    auto v = analogRead(m_VerticalPin);
    auto h = analogRead(m_HorizontalPin);
    auto selection = digitalRead(m_CenterPin) == LOW;
    auto button = selection ? Input::JoystickCenter : Input::JoystickNone;

    Input::JoystickInput input = {
        .x = -Remap((float)h, 0.0f, (float)JOYSTICK_RANGE_X, -1.0f, 1.0f),
        .y = Remap((float)v, 0.0f, (float)JOYSTICK_RANGE_Y, -1.0f, 1.0f),
        .button = button };
    return input;
}
