#ifndef MENU_H
#define MENU_H

#include "src/core/component/component.h"
#include "src/input/event.h"
#include "src/core/time/timer.h"
#include "src/core/menu/menuSystem.h"

using namespace Input;

namespace Slider {

    class Menu : public Core::Component
    {
    public:
        Menu(Core::Display* display, int delay);
        void Update() override;
        bool OnInputEvent(const Input::Event& inputEvent);
        void AddCommand(MenuCommand* command);

    private:
        void OnSelectionLongPress();
        void OnIntroFinished();
        void OutputMenu() const;

        Core::Display* m_Display;
        Core::Timer m_ShowHideTimer;
        Core::Timer m_IntroTimer;
        Core::MenuSystem m_MenuSystem;
        const int m_Delay;
        bool m_IsIntroFinished;
    };
}

#endif
