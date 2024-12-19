#include "dpadSimulator.h"
#include "src/hardware/dpad.h"
#include "src/debug.h"

#include <esp32-hal-gpio.h>

static const int D_LOW = 0;
static const int D_HIGH = 4095;
static const int D_RANGE = 100;

bool IsInBounds(int value, int low, int high) {
    return value >= low && value <= high;
}

Hardware::DpadSimulator::DpadSimulator(
    const int verticalPin, const int horizontalPin, const int selectionPin):
    m_HorizontalPin(horizontalPin),
    m_VerticalPin(verticalPin),
    m_SelectionPin(selectionPin)
{}

void Hardware::DpadSimulator::Init()
{
    pinMode(m_VerticalPin, INPUT);
    pinMode(m_HorizontalPin, INPUT);
    pinMode(m_SelectionPin, INPUT_PULLUP);
}

Hardware::DpadButton Hardware::DpadSimulator::ReadButton() const
{
    auto vertical = analogRead(m_VerticalPin);
    auto horizontal = analogRead(m_HorizontalPin);
    auto selection = digitalRead(m_SelectionPin) == LOW;

    if (IsLeft(horizontal))
        return Hardware::DpadButton::LEFT;
    else if (IsRight(horizontal))
        return Hardware::DpadButton::RIGHT;
    else if (IsUp(vertical))
        return Hardware::DpadButton::UP;
    else if (IsDown(vertical))
        return Hardware::DpadButton::DOWN;
    else if (selection)
        return Hardware::DpadButton::SELECTION;
    else
        return Hardware::DpadButton::NONE;
}

bool Hardware::DpadSimulator::DpadSimulator::IsLeft(const int horizontal)
{
    return IsInBounds(horizontal, D_HIGH - D_RANGE, D_HIGH);
}

bool Hardware::DpadSimulator::DpadSimulator::IsRight(const int horizontal)
{
    return IsInBounds(horizontal, D_LOW, D_LOW + D_RANGE);
}

bool Hardware::DpadSimulator::DpadSimulator::IsUp(const int vertical)
{
    return IsInBounds(vertical, D_HIGH - D_RANGE, D_HIGH);
}

bool Hardware::DpadSimulator::DpadSimulator::IsDown(const int vertical)
{
    return IsInBounds(vertical, D_LOW, D_LOW + D_RANGE);
}