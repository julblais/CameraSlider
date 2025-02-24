#ifndef MENU_H
#define MENU_H

#include "src/core/component/component.h"
#include "src/input/event.h"
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
        Menu(Output::DisplayBuffer* display, int delay);
        void Setup() override;
        bool OnInputEvent(const Input::Event& inputEvent);

    private:
        void OnSelectionLongPress();
        void OnIntroFinished();
        void OutputMenu();

        Output::DisplayBuffer* m_DisplayBuffer;
        Core::Timer m_ShowHideTimer;
        Core::Timer m_IntroTimer;
        Core::MenuSystem m_MenuSystem;
        const int m_Delay;
    };
}

#endif