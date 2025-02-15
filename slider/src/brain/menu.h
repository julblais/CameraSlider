#ifndef MENU_H
#define MENU_H

#include "src/core/component/component.h"
#include "src/input/input.h"
#include "src/core/time/timer.h"
#include "src/core/menu/menuSystem.h"

namespace Output
{
    class DisplayBuffer;
};

using namespace Input;

namespace Slider {

    class Menu : public Core::Component
    {
    public:
        Menu(Core::TimeManager* time, Output::DisplayBuffer* display, int delay);
        void Setup() override;
        bool OnInputEvent(const Input::Event& inputEvent);

    private:
        enum class State { Hidden = 0, Intro = 1, Shown = 2 };

        void OnSelectionLongPress(unsigned long time);
        void OnIntroFinished(unsigned long time);
        void OutputMenu();

        Output::DisplayBuffer* m_DisplayBuffer;
        Core::Timer m_ShowHideTimer;
        Core::Timer m_IntroTimer;
        Core::MenuSystem m_MenuSystem;
        Core::TimeManager* m_Time;
        const int m_Delay;
        State m_State;
    };
}

#endif