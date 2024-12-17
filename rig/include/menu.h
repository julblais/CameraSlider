#ifndef MENU_H
#define MENU_H

#include "hardware/lcd.h"
#include "hardware/dpad.h"
#include "utils/timer.h"

namespace Rig{

class Menu : public Hardware::IDpadListener
{
    public:
        Menu(Hardware::LCD* lcd, Hardware::IDpad* dpad);
        void Init();

        void OnKeyUp(const Hardware::DpadButton button) override;
        void OnKeyDown(const Hardware::DpadButton button) override;

    private:
        void OnOpenMenu(unsigned long time);
        
        Hardware::LCD* m_LCD;
        Hardware::IDpad* m_Dpad;
        Utils::Timer m_Timer;
};

}

#endif