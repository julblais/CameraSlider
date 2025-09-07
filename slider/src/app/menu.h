#ifndef MENU_H
#define MENU_H

#include "src/core/component/component.h"
#include "src/input/event.h"
#include "src/core/time/timer.h"
#include "src/core/menu/menuSystem.h"

using namespace Input;

namespace Slider
{
    class Menu : public Component
    {
    public:
        Menu(Display* display, int delay);
        void Update() override;
        bool OnInputEvent(const Event& inputEvent);
        void AddCommand(MenuCommand* command);

    private:
        void OnSelectionLongPress();
        void OnIntroFinished();
        void OutputMenu() const;

        Display* m_Display;
        const int m_Delay;
        Timer m_ShowHideTimer;
        Timer m_IntroTimer;
        MenuSystem m_MenuSystem;
        bool m_IsIntroFinished;
    };
}

#endif
