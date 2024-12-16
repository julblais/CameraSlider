#ifndef MENU_H
#define MENU_H

#include "lcd.h"
#include "dpad.h"

namespace Rig{

class Menu : public Hardware::IDpadListener
{
    private:
        Hardware::LCD m_LCD;
        Hardware::Dpad* m_Joystick;

    public:
        Menu(Hardware::LCD& lcd, Hardware::Dpad* joystick);
        void Init();

        void OnKeyUp(const Hardware::DpadButton button) override;
        void OnKeyDown(const Hardware::DpadButton button) override;
};

}

#endif