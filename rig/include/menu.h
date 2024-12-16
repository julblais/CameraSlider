#ifndef MENU_H
#define MENU_H

#include "lcd.h"
#include "dpad.h"

namespace Rig{

class Menu
{
private:
  Hardware::LCD m_LCD;
  Hardware::Dpad m_Joystick;

public:
  Menu(Hardware::LCD& lcd, Hardware::Dpad joystick);
};

}

#endif