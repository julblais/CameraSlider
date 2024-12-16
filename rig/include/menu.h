#ifndef MENU_H
#define MENU_H

#include "hardware/lcd.h"
#include "hardware/dpad.h"

namespace Rig{

class Menu : public Hardware::IDpadListener
{
    private:
        Hardware::LCD* m_LCD;
        Hardware::IDpad* m_Joystick;

    public:
        Menu();
        void Init(Hardware::LCD* lcd, Hardware::IDpad* joystick);

        void OnKeyUp(const Hardware::DpadButton button) override;
        void OnKeyDown(const Hardware::DpadButton button) override;
};

}

#endif