#ifndef APP_H
#define APP_H

#include "src/input/input.h"
#include "src/input/inputDispatcher.h"
#include "src/input/inputReader.h"
#include "src/output/display.h"
#include "src/output/displayBuffer.h"
#include "menu.h"

#include <memory>
#include "src/hardware/stepper.h"

namespace Slider
{
    using namespace Input;
    using namespace Output;

    struct AppConfig
    {
        public:
            unsigned char DpadUpPin;
            unsigned char DpadDownPin;
            unsigned char DpadLeftPin;
            unsigned char DpadRightPin;
            unsigned char DpadSelectionPin;
            unsigned char JoystickXPin;
            unsigned char JoystickYPin;
            unsigned char JoystickCenterPin;
            unsigned char StepperDirectionPin;
            unsigned char StepperStepPin;
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
            std::unique_ptr<Display> m_Display;
            std::unique_ptr<IDpadReader> m_Dpad;
            std::unique_ptr<IJoystickReader> m_Joystick;
            std::unique_ptr<Stepper> m_Stepper;
            std::unique_ptr<Menu> m_Menu;
            DisplayBuffer m_DisplayBuffer;
            InputDispatcher m_InputDispatcher;
    };
}

#endif