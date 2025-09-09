#ifndef BRAINAPP_H
#define BRAINAPP_H

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

    class BrainApp : public AppBase
    {
        friend class SimulatorApp;

    public:
        BrainApp(const AppConfig& config);
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
