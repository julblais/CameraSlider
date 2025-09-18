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
      m_IsIntroFinished(true)
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

bool Slider::Menu::OnInputEvent(const Event& inputEvent)
{
    if (HasValue(inputEvent.GetPressedButtons(), ButtonEvent::Select))
        m_ShowHideTimer.Start(m_Delay);
    else if (HasValue(inputEvent.GetReleasedButtons(), ButtonEvent::Select))
        m_ShowHideTimer.Stop();

    if (m_MenuSystem.IsShown())
    {
        if (inputEvent.IsDpadLeft())
            m_MenuSystem.Left();
        if (inputEvent.IsDpadRight())
            m_MenuSystem.Right();
        if (inputEvent.IsDpadUp())
            m_MenuSystem.Up();
        if (inputEvent.IsDpadDown())
            m_MenuSystem.Down();
        if (inputEvent.IsDpadSelect())
            m_MenuSystem.Select();
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
