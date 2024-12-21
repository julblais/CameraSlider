#ifndef MENU_H
#define MENU_H

#include "src/hardware/lcd.h"
#include "src/input/input.h"
#include "src/utils/timer.h"
#include "src/utils/menuSystem.h"

using namespace Input;

namespace Slider{

    class Menu
    {
        public:
            Menu(Hardware::LCD* lcd, unsigned long delay);
            void Init();

            bool OnInputEvent(const Input::Event& inputEvent);

        private:
            void OnSelectionLongPress(unsigned long time);
            void OutputMenu();
            
            Hardware::LCD* m_LCD;
            Utils::Timer m_Timer;
            Utils::MenuSystem m_MenuSystem;
    };

}

#endif