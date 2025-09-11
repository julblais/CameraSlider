#ifndef SIMULATORAPP_H
#define SIMULATORAPP_H

#if IS_SIMULATOR

#include "core/appBase.h"
#include "appConfig.h"
#include "event.h"
#include "core/display.h"
#include "displayBuffer.h"
#include "deviceInputReader.h"
#include "autoInput.h"
#include <memory>

namespace Slider
{
    class SimulatorApp : public Core::AppBase
    {
    public:
        SimulatorApp(const AppConfig& config);
        void Setup() override;
        void Update() override;

    private:
        AppConfig m_Config;
        std::unique_ptr<Core::Display> m_Display;
        std::unique_ptr<Hardware::DeviceInputReader> m_LocalInput;
        std::unique_ptr<IO::AutoInput> m_AutoInput;
        IO::DisplayBuffer m_DisplayBuffer;
        IO::EventDispatcher m_InputDispatcher;
    };
}

#endif
#endif