#include "menu.h"
#include "lcd.h"
#include "joystick.h"

Rig::Menu::Menu(Utils::LCD &lcd, Utils::Joystick joystick)
  :m_LCD(lcd), m_Joystick(joystick)
{
}
