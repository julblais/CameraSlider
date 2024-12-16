#ifndef APP_H
#define APP_H

#include "hardware/lcd.h"
#include "hardware/dpad.h"
#include "menu.h"

namespace Rig
{
    struct AppConfig
    {
        public:
            int DpadVerticalPin;
            int DpadHorizontalPin;
            int DpadSelectionPin;
            int LcdAddress;
    };

    class App
    {
        public:
            App(const AppConfig& config);
            void Setup();
            void Update();

        private:
            AppConfig m_Config;
            Hardware::LCD m_LCD;
            Hardware::Dpad m_Dpad;
            Menu m_Menu;
    };
}


#endif