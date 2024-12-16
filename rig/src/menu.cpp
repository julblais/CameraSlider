#include "menu.h"
#include "hardware/lcd.h"
#include "hardware/dpad.h"
#include "debug.h"

Rig::Menu::Menu()
{}

void Rig::Menu::Init(Hardware::LCD* lcd, Hardware::IDpad* joystick)
{
    m_LCD = lcd;
    m_Joystick = joystick;
    m_Joystick->AddListener(this);
}

void Rig::Menu::OnKeyUp(const Hardware::DpadButton button)
{
    m_LCD->Clear();
}

void Rig::Menu::OnKeyDown(const Hardware::DpadButton button)
{
    m_LCD->Print("Active", 0);
    m_LCD->Print(m_Joystick->PrintButton(button), 1);
}
