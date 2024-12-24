#include "menu.h"
#include "commands.h"

#include "src/debug.h"
#include "src/output/displayBuffer.h"

#define MENU_INTRO_DELAY_MS 1500

Slider::Menu::Menu(Output::DisplayBuffer* display, int delay) :
    m_DisplayBuffer(display),
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
        m_DisplayBuffer->Clear();
        m_MenuSystem.Close();
    }
    else
    {
        Debug.Log("Open menu!", time);
        m_DisplayBuffer->Clear();
        m_DisplayBuffer->PrintLine(0, "   -- Menu --");
        delay(MENU_INTRO_DELAY_MS);
        m_MenuSystem.Open();
        OutputMenu();
    }
}

void Slider::Menu::OutputMenu()
{
    const auto out = m_MenuSystem.GetOutput();
    const auto upDownArrows = m_DisplayBuffer->GetSymbol(Output::Symbol::UpDownArrows);
    const auto leftRightArrows = m_DisplayBuffer->GetSymbol(Output::Symbol::LeftRightArrows);
    m_DisplayBuffer->PrintLine(0, upDownArrows, out.title);
    m_DisplayBuffer->PrintLine(1, " ", leftRightArrows, out.desc);
}
