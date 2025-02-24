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

namespace Net
{
    class WifiModule;
}

using namespace Input;

namespace Slider {

    class Menu : public Core::Component
    {
    public:
        Menu(Net::WifiModule* const wifi, Output::DisplayBuffer* display, int delay);
        void Setup() override;
        bool OnInputEvent(const Input::Event& inputEvent);

    private:
        enum class State { Hidden = 0, Intro = 1, Shown = 2 };

        void OnSelectionLongPress();
        void OnIntroFinished();
        void OutputMenu();

        Output::DisplayBuffer* m_DisplayBuffer;
        Net::WifiModule* m_Wifi;
        Core::Timer m_ShowHideTimer;
        Core::Timer m_IntroTimer;
        Core::MenuSystem m_MenuSystem;
        const int m_Delay;
        State m_State;
    };
}

#endif