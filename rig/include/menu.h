#ifndef MENU_H
#define MENU_H

#include "lcd.h"
#include "joystick.h"

namespace Rig{

class Menu
{
private:
  Hardware::LCD m_LCD;
  Hardware::Joystick m_Joystick;

public:
  Menu(Hardware::LCD& lcd, Hardware::Joystick joystick);
};

}

#endif