#include "hardware/dpad.h"
#include "debug.h"

#include <algorithm>
#include <vector>
#include <esp32-hal-gpio.h>

Hardware::IDpad::IDpad()
    :m_Listeners()
{
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
    return "Invalid";
}
