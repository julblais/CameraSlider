#ifndef APP_H
#define APP_H

#include "src/input/input.h"
#include "src/input/inputDispatcher.h"
#include "src/input/inputReader.h"
#include "src/output/display.h"
#include "src/output/displayBuffer.h"
#include "menu.h"

#include <memory>

namespace Slider
{
    struct AppConfig
    {
        public:
            int DpadUpPin;
            int DpadDownPin;
            int DpadLeftPin;
            int DpadRightPin;
            int DpadSelectionPin;
            int JoystickXPin;
            int JoystickYPin;
            int JoystickCenterPin;
            int LcdAddress;
            int ShowMenuDelayMs;
    };

    class App
    {
        public:
            App(const AppConfig& config);
            void Setup();
            void Update();

        private:
            void SetupComponents(const AppConfig &config);

            AppConfig m_Config;
            std::unique_ptr<Output::Display> m_Display;
            std::unique_ptr<Input::IDpadReader> m_Dpad;
            std::unique_ptr<Input::IJoystickReader> m_Joystick;
            std::unique_ptr<Menu> m_Menu;
            Output::DisplayBuffer m_DisplayBuffer;
            Input::InputDispatcher m_InputDispatcher;
    };
}

#endif