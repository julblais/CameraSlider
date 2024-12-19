#ifndef APP_H
#define APP_H

#include "src/hardware/lcd.h"
#include "src/input/input.h"
#include "src/input/inputDispatcher.h"
#include "menu.h"

#include <memory>

namespace Slider
{
    struct AppConfig
    {
        public:
            bool isSimulator;
            int DpadVerticalPin;
            int DpadHorizontalPin;
            int DpadSelectionPin;
            int LcdAddress;
            int showMenuDelayMs;
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
            std::unique_ptr<Input::IDpadReader> m_Dpad;
            std::unique_ptr<Menu> m_Menu;
            Input::InputDispatcher m_InputDispatcher;
    };
}

#endif