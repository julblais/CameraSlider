#include "menu.h"
#include "lcd.h"
#include "dpad.h"
#include "debug.h"

Rig::Menu::Menu(Hardware::LCD* const lcd, Hardware::Dpad* const joystick)
    :m_LCD(lcd), m_Joystick(joystick)
{}

void Rig::Menu::Init()
{
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
