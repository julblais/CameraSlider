#ifndef APP_H
#define APP_H

#include "src/core/app/appBase.h"
#include "src/app/appConfig.h"
#include "src/app/menu.h"
#include "src/input/input.h"
#include "src/input/event.h"
#include "src/core/output/display.h"
#include "src/output/displayBuffer.h"

#include <memory>
#include "src/hardware/stepper.h"

namespace Slider
{
    using namespace Input;
    using namespace Output;

    class SimulatorApp : public Core::AppBase
    {
    public:
        SimulatorApp(const AppConfig& config);
        virtual void Setup() override;
        virtual void Update() override;

    private:
        void AddDeviceMessageSimulator();
        AppConfig m_Config;
        std::unique_ptr<Core::Display> m_Display;
        std::unique_ptr<Input::InputReader> m_InputReader;
        DisplayBuffer m_DisplayBuffer;
        EventDispatcher m_InputDispatcher;
    };
}

#endif