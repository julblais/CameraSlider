#ifndef MENU_H
#define MENU_H

#include "core/component.h"
#include "event.h"
#include "core/timer.h"
#include "core/menuSystem.h"

namespace Slider
{
    class Menu : public Core::Component
    {
    public:
        Menu(Core::Display* display, int delay);
        void Update() override;
        bool OnInputEvent(const IO::Event& inputEvent);
        void AddCommand(Core::MenuCommand* command);

    private:
        void OnSelectionLongPress();
        void OnIntroFinished();
        void OutputMenu() const;

        Core::Display* m_Display;
        const int m_Delay;
        Core::Timer m_ShowHideTimer;
        Core::Timer m_IntroTimer;
        Core::MenuSystem m_MenuSystem;
        bool m_IsIntroFinished;
    };
}

#endif
