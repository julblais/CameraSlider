#ifndef MENU_H
#define MENU_H

#include "lcd.h"
#include "dpad.h"

namespace Rig{

class Menu : public Hardware::IDpadListener
{
    private:
        Hardware::LCD* const m_LCD;
        Hardware::Dpad* const m_Joystick;

    public:
        Menu(Hardware::LCD* const lcd, Hardware::Dpad* const joystick);
        void Init();

        void OnKeyUp(const Hardware::DpadButton button) override;
        void OnKeyDown(const Hardware::DpadButton button) override;
};

}

#endif