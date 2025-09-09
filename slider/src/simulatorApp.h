#ifndef SIMULATORAPP_H
#define SIMULATORAPP_H

#if IS_SIMULATOR

#include "core/appBase.h"
#include "appConfig.h"
#include "input.h"
#include "event.h"
#include "core/display.h"
#include "displayBuffer.h"

#include <memory>

namespace Slider
{
    using namespace Input;
    using namespace Output;

    class SimulatorApp : public AppBase
    {
    public:
        SimulatorApp(const AppConfig& config);
        void Setup() override;
        void Update() override;

    private:
        AppConfig m_Config;
        std::unique_ptr<Display> m_Display;
        std::unique_ptr<InputReader> m_InputReader;
        DisplayBuffer m_DisplayBuffer;
        EventDispatcher m_InputDispatcher;
    };
}

#endif
#endif