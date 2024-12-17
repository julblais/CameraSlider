#include "menu.h"
#include "hardware/lcd.h"
#include "hardware/dpad.h"
#include "debug.h"
#include "utils/timer.h"

#define MENU_DELAY_MS 2000

Rig::Menu::Menu() :
    m_Timer("Selection menu", [this](unsigned long time){ OnOpenMenu(time); }, MENU_DELAY_MS)
{}

void Rig::Menu::Init(Hardware::LCD* lcd, Hardware::IDpad* dpad)
{
    m_LCD = lcd;
    m_Dpad = dpad;
    m_Dpad->AddListener(this);
}

void Rig::Menu::OnKeyUp(const Hardware::DpadButton button)
{
    m_LCD->Clear();
    m_Timer.Stop();
}

void Rig::Menu::OnKeyDown(const Hardware::DpadButton button)
{
    if(button == Hardware::DpadButton::SELECTION)
    {
        m_Timer.Start();
    }
    else
    {
        m_LCD->Print("Active", 0);
        m_LCD->Print(m_Dpad->PrintButton(button), 1);
    }
}

void Rig::Menu::OnOpenMenu(unsigned long time)
{
    Debug.Log("Open menu! ", time);
    m_LCD->Print("Menu", 0);
    m_LCD->Print("", 1);
}
