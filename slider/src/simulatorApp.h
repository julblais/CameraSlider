#ifndef SIMULATORAPP_H
#define SIMULATORAPP_H

#if IS_SIMULATOR

#include "core/appBase.h"
#include "appConfig.h"
#include "core/display.h"
#include "displayBuffer.h"
#include "deviceInputReader.h"
#include "autoInput.h"
#include "event.h"
#include <memory>

#include "eventDispatcher.h"

namespace Slider
{
    class SimulatorApp : public Core::AppBase
    {
    public:
        SimulatorApp(const AppConfig& config);
        void Setup() override;
        void Update() override;
        bool OnInputEvent(const IO::Event& inputEvent);

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