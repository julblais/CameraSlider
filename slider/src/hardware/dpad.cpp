#include "dpad.h"
#include "src/debug.h"

Hardware::Dpad::Dpad(int upPin, int downPin, int leftPin, int rightPin, int selectionPin) :
    m_UpPin(upPin),
    m_DownPin(downPin),
    m_LeftPin(leftPin),
    m_RightPin(rightPin),
    m_SelectionPin(selectionPin)
{}

void Hardware::Dpad::Init()
{
    pinMode(m_UpPin, INPUT_PULLUP);
    pinMode(m_DownPin, INPUT_PULLUP);
    pinMode(m_LeftPin, INPUT_PULLUP);
    pinMode(m_RightPin, INPUT_PULLUP);
    pinMode(m_SelectionPin, INPUT_PULLUP);
}

Input::DpadInput Hardware::Dpad::ReadInput()
{
    auto up = digitalRead(m_UpPin) == LOW;
    auto down = digitalRead(m_DownPin) == LOW;
    auto left = digitalRead(m_LeftPin) == LOW;
    auto right = digitalRead(m_RightPin) == LOW;
    auto selection = digitalRead(m_SelectionPin) == LOW;

    auto input = Input::DpadNone;
    if (left)
        input = Input::DpadLeft;
    else if (right)
        input = Input::DpadRight;
    else if (up)
        input = Input::DpadUp;
    else if (down)
        input = Input::DpadDown;
    else if (selection)
        input = Input::DpadSelect;

    Input::DpadInput result = { .button = input };
    return result;
}