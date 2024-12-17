#include "hardware/dpad.h"
#include "debug.h"

#include <algorithm>
#include <vector>
#include <esp32-hal-gpio.h>

Hardware::IDpad::State::State(DpadButton button, int delta):
    button(button),
    delta(delta)
{}

Hardware::IDpad::IDpad()
    :m_Listeners()
{}

void Hardware::IDpad::Init()
{
    m_LastState = CreateState(ReadButton());
}

void Hardware::IDpad::Update() 
{
    auto currentState = CreateState(ReadButton());
    ProcessButton(currentState);
    m_LastState = currentState;
}

void Hardware::IDpad::AddListener(IDpadListener *listener)
{
    m_Listeners.push_back(listener);
}

void Hardware::IDpad::RemoveListener(IDpadListener* listener)
{
    auto position = std::find(m_Listeners.begin(), m_Listeners.end(), listener);
    if (position != m_Listeners.end())
        m_Listeners.erase(position);
}

void Hardware::IDpad::SendUpEvent(const DpadButton button) const
{
    Debug.Log("Sending button up: ", PrintButton(button));
    for(auto listener : m_Listeners)
        listener->OnKeyUp(button);
}

void Hardware::IDpad::SendDownEvent(const DpadButton button) const
{
    Debug.Log("Sending button down: ", PrintButton(button));
    for(auto listener : m_Listeners)
        listener->OnKeyDown(button);
}

const char* Hardware::IDpad::PrintButton(const DpadButton button)
{
    switch(button) {
        case Hardware::DpadButton::LEFT:
            return "left";
        case Hardware::DpadButton::UP:
            return "up";
        case Hardware::DpadButton::DOWN:
            return "down";
        case Hardware::DpadButton::RIGHT:
            return "right";
        case Hardware::DpadButton::SELECTION:
            return "selection";
    }
    return "None";
}

Hardware::IDpad::State Hardware::IDpad::CreateState(Hardware::DpadButton button) const
{
    return State(button, 0);
}

void Hardware::IDpad::ProcessButton(const Hardware::IDpad::State& state) const
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

    if (!m_LastState.IsSelection() && state.IsSelection()) //select pressed
        SendDownEvent(Hardware::DpadButton::SELECTION);
    else if (m_LastState.IsSelection() && !state.IsSelection()) //select depressed
        SendUpEvent(Hardware::DpadButton::SELECTION);
}
