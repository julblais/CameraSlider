#ifndef APP_H
#define APP_H

#include "src/core/app/appBase.h"
#include "appConfig.h"
#include "menu.h"
#include "src/input/input.h"
#include "src/input/inputDispatcher.h"
#include "src/input/inputReader.h"
#include "src/output/display.h"
#include "src/output/displayBuffer.h"

#include <memory>
#include "src/hardware/stepper.h"

namespace Slider
{
    using namespace Input;
    using namespace Output;

    class App : public AppBase
    {
    public:
        App(const AppConfig& config);
        virtual void Setup() override;
        virtual void Update() override;

    private:
        AppConfig m_Config;
        std::unique_ptr<Display> m_Display;
        std::unique_ptr<IDpadReader> m_Dpad;
        std::unique_ptr<IJoystickReader> m_Joystick;
        DisplayBuffer m_DisplayBuffer;
        InputDispatcher m_InputDispatcher;
    };
}

#endif