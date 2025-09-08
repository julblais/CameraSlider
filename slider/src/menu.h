#ifndef MENU_H
#define MENU_H

#include "core/component.h"
#include "event.h"
#include "core/timer.h"
#include "core/menuSystem.h"

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
