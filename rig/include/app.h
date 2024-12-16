#ifndef APP_H
#define APP_H

#include "hardware/lcd.h"
#include "hardware/dpad.h"
#include "menu.h"

#include <memory>

namespace Rig
{
    constexpr bool IS_SIMULATOR = true;

    struct AppConfig
    {
        public:
            bool isSimulator = IS_SIMULATOR;
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
            std::unique_ptr<Hardware::LCD> m_LCD;
            std::unique_ptr<Hardware::IDpad> m_Dpad;
            std::unique_ptr<Menu> m_Menu;
    };
}


#endif