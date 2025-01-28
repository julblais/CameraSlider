#include "menu.h"
#include "commands.h"
#include "settings.h"

#include "src/debug.h"
#include "src/output/displayBuffer.h"

#define MENU_INTRO_DELAY_MS 1500
#define MENU_INTRO_MSG "   -- Menu --"

Slider::Menu::Menu(Output::DisplayBuffer* display, int delay) :
    m_DisplayBuffer(display),
    m_ShowHideTimer("Selection menu", [this](unsigned long time){ OnSelectionLongPress(time); }, delay),
    m_IntroTimer("Intro menu", [this](unsigned long time){ OnIntroFinished(time); }, MENU_INTRO_DELAY_MS),
    m_MenuSystem(),
    m_State(State::Hidden)
{}

void Slider::Menu::Init()
{    
    m_MenuSystem.AddCommand(new MaxSpeedCommand());
    m_MenuSystem.AddCommand(new SpeedCurveCommand());
}

bool Slider::Menu::OnInputEvent(const Input::Event &inputEvent)
{
    if (inputEvent.dpadButtonState == Input::ButtonReleased)
    {
        m_ShowHideTimer.Stop();
        return false;
    }
    
    if (inputEvent.dpadButtonState == Input::ButtonPressed)
    {
        if (inputEvent.button == Input::DpadSelect)
            m_ShowHideTimer.Start();
        if (m_State == State::Shown)
        {
            switch(inputEvent.button)
            {
                case Input::DpadLeft:
                    m_MenuSystem.Left();
                    break;
                case Input::DpadRight:
                    m_MenuSystem.Right();
                    break;
                case Input::DpadUp:
                    m_MenuSystem.Up();
                    break;
                case Input::DpadDown:
                    m_MenuSystem.Down();
                    break;
                case Input::DpadSelect:
                    m_MenuSystem.Select();
                default:
                    break;
            };
            OutputMenu();
        }
    }
    
    return m_State != State::Hidden;
}

void Slider::Menu::OnSelectionLongPress(unsigned long time)
{
    if (m_State == State::Hidden) //show intro
    {
        m_State = State::Intro;
        m_IntroTimer.Start();
        m_DisplayBuffer->Clear();
        m_DisplayBuffer->Print(MENU_INTRO_MSG);
    }
    else { //quit menu
        m_State = State::Hidden;
        m_IntroTimer.Stop();
        m_DisplayBuffer->Clear();
    }
}

void Slider::Menu::OnIntroFinished(unsigned long time)
{
    //show menu
    m_State = State::Shown;
    m_MenuSystem.Reset();
    OutputMenu();
}

void Slider::Menu::OutputMenu()
{
    const auto out = m_MenuSystem.GetOutput();
    const auto upDownArrows = m_DisplayBuffer->GetSymbol(Output::Symbol::UpDownArrows);
    const auto leftRightArrows = m_DisplayBuffer->GetSymbol(Output::Symbol::LeftRightArrows);
    m_DisplayBuffer->PrintLine(0, upDownArrows, out.title);
    m_DisplayBuffer->PrintLine(1, " ", leftRightArrows, out.desc);
}
