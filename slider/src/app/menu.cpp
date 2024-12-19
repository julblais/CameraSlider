#include "menu.h"
#include "commands.h"

#include "src/hardware/lcd.h"
#include "src/hardware/dpad.h"
#include "src/utils/timer.h"
#include "src/debug.h"

#define MENU_DELAY_MS 2000

Slider::Menu::Menu(Hardware::LCD* lcd, Hardware::IDpad* dpad) :
    m_LCD(lcd),
    m_Dpad(dpad),
    m_Timer("Selection menu", [this](unsigned long time){ OnSelectionLongPress(time); }, MENU_DELAY_MS),
    m_MenuSystem()
{}

void Slider::Menu::Init()
{
    m_Dpad->AddListener(this);
    
    m_MenuSystem.AddCommand(new MaxSpeedCommand());
    m_MenuSystem.AddCommand(new SpeedCurveCommand());
}

void Slider::Menu::OnKeyUp(const Hardware::DpadButton button)
{
    m_Timer.Stop();
}

void Slider::Menu::OnKeyDown(const Hardware::DpadButton button)
{
    if(button == Hardware::DpadButton::SELECTION)
        m_Timer.Start();
    else if(button == Hardware::DpadButton::LEFT)
        m_MenuSystem.MoveLeft();
    else if(button == Hardware::DpadButton::RIGHT)
        m_MenuSystem.MoveRight();
    else if(button == Hardware::DpadButton::UP)
        m_MenuSystem.MoveUp();
    else if(button == Hardware::DpadButton::DOWN)
        m_MenuSystem.MoveDown();

    OutputMenu();
}

void Slider::Menu::OnSelectionLongPress(unsigned long time)
{
    if (m_MenuSystem.IsOpened())
    {
        Debug.Log("Close menu!", time);
        m_LCD->Clear();
        m_MenuSystem.Close();
    }
    else
    {
        Debug.Log("Open menu!", time);
        m_LCD->Clear();
        m_LCD->PrintLn("   -- Menu --", 0);
        delay(1500);
        m_MenuSystem.Open();
        OutputMenu();
    }
}

void Slider::Menu::OutputMenu()
{
    if (m_MenuSystem.IsOpened())
    {
        auto out = m_MenuSystem.GetOutput();
        m_LCD->PrintLn(m_LCD->GetDoubleUpDownArrows(), out.title, 0);
        m_LCD->PrintLn(" ", m_LCD->GetDoubleLeftRightArrows(), out.desc, 1);
    }
}
