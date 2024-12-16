#include "hardware/dpad.h"
#include "hardware/dpadSimulator.h"
#include "debug.h"

#include <algorithm>
#include <vector>
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
    m_SelectionPin(selectionPin),
    m_LastState(D_HIGH/2, D_HIGH/2, false)
{}

void Hardware::DpadSimulator::Init()
{
    pinMode(m_VerticalPin, INPUT);
    pinMode(m_HorizontalPin, INPUT);
    pinMode(m_SelectionPin, INPUT_PULLUP);
}

int Hardware::DpadSimulator::ReadVertical() const
{
    return analogRead(m_VerticalPin);
}

int Hardware::DpadSimulator::ReadHorizontal() const
{
    return analogRead(m_HorizontalPin);
}

bool Hardware::DpadSimulator::SelectionPressed() const
{
    return digitalRead(m_SelectionPin) == LOW;
}

void Hardware::DpadSimulator::Update()
{
    auto currentState = ReadState();

    ProcessButtons(currentState);

    m_LastState = currentState;
}

Hardware::DpadSimulator::State Hardware::DpadSimulator::ReadState() const
{
    return State(ReadVertical(), ReadHorizontal(), SelectionPressed());
}

void Hardware::DpadSimulator::ProcessButtons(const State &state) const
{
    if (!m_LastState.IsDown() && state.IsDown()) //key down pressed
        SendDownEvent(Hardware::DpadButton::DOWN);
    else if (m_LastState.IsDown() && !state.IsDown()) //key down depressed
        SendUpEvent(Hardware::DpadButton::DOWN);

    if (!m_LastState.IsUp() && state.IsUp()) //key up pressed
        SendDownEvent(Hardware::DpadButton::UP);
    else if (m_LastState.IsUp() && !state.IsUp()) //key up depressed
        SendUpEvent(Hardware::DpadButton::UP);

    if (!m_LastState.IsLeft() && state.IsLeft()) //key left pressed
        SendDownEvent(Hardware::DpadButton::LEFT);
    else if (m_LastState.IsLeft() && !state.IsLeft()) //key left depressed
        SendUpEvent(Hardware::DpadButton::LEFT);
    
    if (!m_LastState.IsRight() && state.IsRight()) //key right pressed
        SendDownEvent(Hardware::DpadButton::RIGHT);
    else if (m_LastState.IsRight() && !state.IsRight()) //key right depressed
        SendUpEvent(Hardware::DpadButton::RIGHT);

    if (!m_LastState.Selection && state.Selection) //select pressed
        SendDownEvent(Hardware::DpadButton::SELECTION);
    else if (m_LastState.Selection && !state.Selection) //select depressed
        SendUpEvent(Hardware::DpadButton::SELECTION);
}


Hardware::DpadSimulator::State::State(const int vertical, const int horizontal, const int selection)
{
    Vertical = vertical;
    Horizontal = horizontal;
    Selection = selection;
}

bool Hardware::DpadSimulator::State::IsLeft() const
{
    return IsInBounds(Horizontal, D_HIGH - D_RANGE, D_HIGH);
}

bool Hardware::DpadSimulator::State::IsRight() const
{
    return IsInBounds(Horizontal, D_LOW, D_LOW + D_RANGE);
}

bool Hardware::DpadSimulator::State::IsUp() const
{
    return IsInBounds(Vertical, D_HIGH - D_RANGE, D_HIGH);
}

bool Hardware::DpadSimulator::State::IsDown() const
{
    return IsInBounds(Vertical, D_LOW, D_LOW + D_RANGE);
}