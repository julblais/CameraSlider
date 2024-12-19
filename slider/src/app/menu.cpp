#include "menu.h"
#include "commands.h"

#include "src/debug.h"

#define MENU_DELAY_MS 2000

using namespace Input;

Slider::Menu::Menu(Hardware::LCD* lcd) :
    m_LCD(lcd),
    m_Timer("Selection menu", [this](unsigned long time){ OnSelectionLongPress(time); }, MENU_DELAY_MS),
    m_MenuSystem()
{}

void Slider::Menu::Init()
{    
    m_MenuSystem.AddCommand(new MaxSpeedCommand());
    m_MenuSystem.AddCommand(new SpeedCurveCommand());
}

bool Slider::Menu::OnButtonReleased(const DpadButton button)
{
    m_Timer.Stop();
    return true; //capture quit menu
}

bool Slider::Menu::OnButtonPressed(const DpadButton button)
{
    if (button == DpadNone)
        return false;
        
    if(button == DpadSelect)
    {
        m_Timer.Start();
        return true;
    }

    if(button == DpadLeft)
        m_MenuSystem.MoveLeft();
    else if(button == DpadRight)
        m_MenuSystem.MoveRight();
    else if(button == DpadUp)
        m_MenuSystem.MoveUp();
    else if(button == DpadDown)
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
