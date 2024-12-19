#include "dpadSimulator.h"
#include "src/debug.h"

#include <esp32-hal-gpio.h>

static const int D_LOW = 0;
static const int D_HIGH = 4095;
static const int D_RANGE = 100;

bool IsInBounds(int value, int low, int high) {
    return value >= low && value <= high;
}

Simulator::DpadSimulator::DpadSimulator(
    const int verticalPin, const int horizontalPin, const int selectionPin):
    m_HorizontalPin(horizontalPin),
    m_VerticalPin(verticalPin),
    m_SelectionPin(selectionPin)
{}

void Simulator::DpadSimulator::Init()
{
    pinMode(m_VerticalPin, INPUT);
    pinMode(m_HorizontalPin, INPUT);
    pinMode(m_SelectionPin, INPUT_PULLUP);
}

Input::DpadInput Simulator::DpadSimulator::ReadInput()
{
    auto vertical = analogRead(m_VerticalPin);
    auto horizontal = analogRead(m_HorizontalPin);
    auto selection = digitalRead(m_SelectionPin) == LOW;

    auto input = Input::DpadNone;
    if (IsLeft(horizontal))
        input = Input::DpadLeft;
    else if (IsRight(horizontal))
        input = Input::DpadRight;
    else if (IsUp(vertical))
        input = Input::DpadUp;
    else if (IsDown(vertical))
        input = Input::DpadDown;
    else if (selection)
        input = Input::DpadSelect;

    return Input::DpadInput(input);
}

bool Simulator::DpadSimulator::DpadSimulator::IsLeft(const int horizontal)
{
    return IsInBounds(horizontal, D_HIGH - D_RANGE, D_HIGH);
}

bool Simulator::DpadSimulator::DpadSimulator::IsRight(const int horizontal)
{
    return IsInBounds(horizontal, D_LOW, D_LOW + D_RANGE);
}

bool Simulator::DpadSimulator::DpadSimulator::IsUp(const int vertical)
{
    return IsInBounds(vertical, D_HIGH - D_RANGE, D_HIGH);
}

bool Simulator::DpadSimulator::DpadSimulator::IsDown(const int vertical)
{
    return IsInBounds(vertical, D_LOW, D_LOW + D_RANGE);
}