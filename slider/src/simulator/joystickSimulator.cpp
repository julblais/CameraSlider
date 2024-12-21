#include "joystickSimulator.h"

#include <esp32-hal-gpio.h>

constexpr auto JOYSTICK_RANGE_X = 4095;
constexpr auto JOYSTICK_RANGE_Y = 4095;
constexpr auto JOYSTICK_REMAP_X = (JOYSTICK_RANGE_X + 1) / 2;
constexpr auto JOYSTICK_REMAP_Y = (JOYSTICK_RANGE_Y + 1) / 2;

Simulator::JoystickSimulator::JoystickSimulator(int horizontalPin, int verticalPin, int centerPin):
    m_HorizontalPin(horizontalPin),
    m_VerticalPin(verticalPin),
    m_CenterPin(centerPin)
{}

void Simulator::JoystickSimulator::Init()
{
    pinMode(m_VerticalPin, INPUT);
    pinMode(m_HorizontalPin, INPUT);
    pinMode(m_CenterPin, INPUT_PULLUP);
}

Input::JoystickInput Simulator::JoystickSimulator::ReadInput()
{ 
    auto vertical = analogRead(m_VerticalPin) - JOYSTICK_REMAP_Y;
    auto horizontal = analogRead(m_HorizontalPin) - JOYSTICK_REMAP_X;
    auto selection = digitalRead(m_CenterPin) == LOW;
    auto button = selection ? Input::JoystickCenter : Input::JoystickNone;

    Input::JoystickInput input = { .x = horizontal, .y = vertical, .button = button };
    return input;
}
