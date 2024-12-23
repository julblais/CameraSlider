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
            void OnSelectionLongPress(unsigned long time);
            void OutputMenu();
            
            Output::DisplayBuffer* m_DisplayBuffer;
            Utils::Timer m_Timer;
            Utils::MenuSystem m_MenuSystem;
    };

}

#endif