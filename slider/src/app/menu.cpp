#include "menu.h"
#include "commands.h"

#include "src/debug.h"

#define MENU_INTRO_DELAY_MS 1500

Slider::Menu::Menu(Hardware::LCD* lcd, unsigned long delay) :
    m_LCD(lcd),
    m_Timer("Selection menu", [this](unsigned long time){ OnSelectionLongPress(time); }, delay),
    m_MenuSystem()
{}

void Slider::Menu::Init()
{    
    m_MenuSystem.AddCommand(new MaxSpeedCommand());
    m_MenuSystem.AddCommand(new SpeedCurveCommand());
}

bool Slider::Menu::OnButtonReleased(const Input::DpadButton button)
{
    m_Timer.Stop();
    return true; //capture quit menu
}

bool Slider::Menu::OnButtonPressed(const Input::DpadButton button)
{
    if (button == Input::DpadNone)
        return false;
        
    if(button == Input::DpadSelect)
    {
        m_Timer.Start();
        return true;
    }

    if(button == Input::DpadLeft)
        m_MenuSystem.MoveLeft();
    else if(button == Input::DpadRight)
        m_MenuSystem.MoveRight();
    else if(button == Input::DpadUp)
        m_MenuSystem.MoveUp();
    else if(button == Input::DpadDown)
        m_MenuSystem.MoveDown();

    OutputMenu();
    return m_MenuSystem.IsOpened();
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
        delay(MENU_INTRO_DELAY_MS);
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
