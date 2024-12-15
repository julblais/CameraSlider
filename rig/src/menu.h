#ifndef MENU_H
#define MENU_H

#include "lcd.h"

namespace Rig{

class Menu
{
private:
  Utils::LCD m_LCD;

public:
  Menu(Utils::LCD& lcd);
};

}

#endif