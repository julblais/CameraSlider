#include "joystickSimulator.h"

#include <esp32-hal-gpio.h>

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
    auto vertical = analogRead(m_VerticalPin);
    auto horizontal = analogRead(m_HorizontalPin);
    auto selection = digitalRead(m_CenterPin) == LOW;
    auto button = selection ? Input::JoystickCenter : Input::JoystickNone;

    return Input::JoystickInput(horizontal, vertical, button);
}
