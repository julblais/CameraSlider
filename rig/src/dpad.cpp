#include "dpad.h"

#include <esp32-hal-gpio.h>

Hardware::Dpad::Dpad(const int verticalPin, const int horizontalPin, const int selectionPin)
    :m_HorizontalPin(horizontalPin), m_VerticalPin(verticalPin), m_SelectionPin(selectionPin)
{
}

void Hardware::Dpad::Init()
{
    pinMode(m_VerticalPin, INPUT);
    pinMode(m_HorizontalPin, INPUT);
    pinMode(m_SelectionPin, INPUT_PULLUP);
}

// vert goes from 0 (bottom) to 1023 (top)
int Hardware::Dpad::ReadVertical()
{
    return analogRead(m_VerticalPin);
}

// horz goes from 0 (right) to 1023 (left)
int Hardware::Dpad::ReadHorizontal()
{
    return analogRead(m_HorizontalPin);
}

bool Hardware::Dpad::SelectionPressed()
{
    return digitalRead(m_SelectionPin) == LOW;
}
