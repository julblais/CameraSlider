#ifndef MENU_H
#define MENU_H

#include "src/input/input.h"
#include "src/utils/timer.h"
#include "src/utils/menuSystem.h"

namespace Output
{
    class DisplayBuffer;
};

using namespace Input;

namespace Slider{

    class Menu
    {
        public:
            Menu(Output::DisplayBuffer* display, int delay);
            void Init();
            bool OnInputEvent(const Input::Event& inputEvent);

        private:
            enum class State { Hidden = 0, Intro = 1, Shown = 2 };

            void OnSelectionLongPress(unsigned long time);
            void OnIntroFinished(unsigned long time);
            void OutputMenu();
            
            Output::DisplayBuffer* m_DisplayBuffer;
            Utils::Timer m_ShowHideTimer;
            Utils::Timer m_IntroTimer;
            Utils::MenuSystem m_MenuSystem;
            State m_State;
    };
}

#endif