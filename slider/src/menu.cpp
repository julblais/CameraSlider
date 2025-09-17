#include "menu.h"

#include "core/debug.h"
#include "core/display.h"

#define MENU_INTRO_DELAY_MS 1500
#define MENU_INTRO_MSG "   -- Menu --"

using namespace Core;
using namespace IO;

Slider::Menu::Menu(Display* display, const int delay)
    : m_Display(display),
      m_Delay(delay),
      m_IsIntroFinished(false)
{
    m_ShowHideTimer = Timer::Create("Selection menu", [this]() {
        OnSelectionLongPress();
    });
    m_IntroTimer = Timer::Create("Intro menu", [this]() {
        OnIntroFinished();
    });
}

void Slider::Menu::Update()
{
    m_MenuSystem.Update();
    if (m_MenuSystem.IsShown())
        OutputMenu();
}

bool Slider::Menu::OnInputEvent(const Event &inputEvent)
{
    const auto buttonChange = inputEvent.GetButtonChange();
    if (buttonChange == ButtonReleased)
    {
        m_ShowHideTimer.Stop();
    }

    if (buttonChange == ButtonPressed)
    {
        auto button = inputEvent.GetButtonEvent();
        if (button == DpadSelect)
            m_ShowHideTimer.Start(m_Delay);
        if (m_MenuSystem.IsShown())
        {
            switch (button)
            {
                case DpadLeft:
                    m_MenuSystem.Left();
                    break;
                case DpadRight:
                    m_MenuSystem.Right();
                    break;
                case DpadUp:
                    m_MenuSystem.Up();
                    break;
                case DpadDown:
                    m_MenuSystem.Down();
                    break;
                case DpadSelect:
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
    } else
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
