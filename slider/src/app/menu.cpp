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

bool Slider::Menu::OnInputEvent(const Input::Event &inputEvent)
{
    if (inputEvent.dpadButtonState == Input::ButtonReleased)
        m_Timer.Stop();
    else if (inputEvent.dpadButtonState == Input::ButtonPressed)
    {
        switch(inputEvent.button)
        {
            case Input::DpadLeft:
                m_MenuSystem.MoveLeft();
                break;
            case Input::DpadRight:
                m_MenuSystem.MoveRight();
                break;
            case Input::DpadUp:
                m_MenuSystem.MoveUp();
                break;
            case Input::DpadDown:
                m_MenuSystem.MoveDown();
                break;
            case Input::DpadSelect:
                m_Timer.Start();
                break;
            default:
                break;
        }
        if (m_MenuSystem.IsOpened())
            OutputMenu();
    }
    
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
    auto out = m_MenuSystem.GetOutput();
    m_LCD->PrintLn(m_LCD->GetDoubleUpDownArrows(), out.title, 0);
    m_LCD->PrintLn(" ", m_LCD->GetDoubleLeftRightArrows(), out.desc, 1);
}
