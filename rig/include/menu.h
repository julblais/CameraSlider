#ifndef MENU_H
#define MENU_H

#include "lcd.h"
#include "joystick.h"

namespace Rig{

class Menu
{
private:
  Utils::LCD m_LCD;
  Utils::Joystick m_Joystick;

public:
  Menu(Utils::LCD& lcd, Utils::Joystick joystick);
};

}

#endif