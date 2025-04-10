#include "menu.h"
#include "settings.h"

#include "src/debug.h"
#include "src/core/output/display.h"

#define MENU_INTRO_DELAY_MS 1500
#define MENU_INTRO_MSG "   -- Menu --"

using namespace Core;

Slider::Menu::Menu(Display* display, int delay) :
    m_Delay(delay),
    m_Display(display),
    m_ShowHideTimer(),
    m_IntroTimer(),
    m_MenuSystem(),
    m_IsIntroFinished(false)
{
    m_ShowHideTimer = Timer::Create("Selection menu", [this]() {
        OnSelectionLongPress(); });
    m_IntroTimer = Timer::Create("Intro menu", [this]() {
        OnIntroFinished(); });
}

void Slider::Menu::Update()
{
    m_MenuSystem.Update();
    if (m_MenuSystem.IsShown())
        OutputMenu();
}

bool Slider::Menu::OnInputEvent(const Input::Event& evt)
{
    auto buttonChange = evt.GetButtonChange();
    if (buttonChange == Input::ButtonReleased)
    {
        m_ShowHideTimer.Stop();
    }

    if (buttonChange == Input::ButtonPressed)
    {
        auto button = evt.GetButtonEvent();
        if (button == Input::DpadSelect)
            m_ShowHideTimer.Start(m_Delay);
        if (m_MenuSystem.IsShown())
        {
            switch (button)
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
        }
    }

    return !m_MenuSystem.IsHidden() || !m_IsIntroFinished;
}

void Slider::Menu::AddCommand(MenuCommand* command)
{
    m_MenuSystem.AddCommand(command);
}

void Slider::Menu::OnSelectionLongPress()
{
    if (m_MenuSystem.IsHidden()) //show intro
    {
        m_IsIntroFinished = false;
        m_IntroTimer.Start(MENU_INTRO_DELAY_MS);
        m_Display->Clear();
        m_Display->Print(MENU_INTRO_MSG);
    }
    else
    {
        m_MenuSystem.Close();
        m_IntroTimer.Stop();
        m_Display->Clear();
    }
}

void Slider::Menu::OnIntroFinished()
{
    m_IsIntroFinished = true;
    m_MenuSystem.Open();
    OutputMenu();
}

void Slider::Menu::OutputMenu() const
{
    m_MenuSystem.Print(m_Display);
}
