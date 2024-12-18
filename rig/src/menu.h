#ifndef MENU_H
#define MENU_H

#include "hardware/lcd.h"
#include "hardware/dpad.h"
#include "utils/timer.h"
#include "utils/menuSystem.h"

namespace Rig{

class Menu : public Hardware::IDpadListener
{
    public:
        Menu(Hardware::LCD* lcd, Hardware::IDpad* dpad);
        void Init();

        void OnKeyUp(const Hardware::DpadButton button) override;
        void OnKeyDown(const Hardware::DpadButton button) override;

    private:
        void OnSelectionLongPress(unsigned long time);
        void OutputMenu();
        
        Hardware::LCD* m_LCD;
        Hardware::IDpad* m_Dpad;
        Utils::Timer m_Timer;
        Utils::MenuSystem m_MenuSystem;
};

}

#endif