#ifndef CONTROLLERAPP_H
#define CONTROLLERAPP_H

#include "src/core/app/appBase.h"
#include "src/app/appConfig.h"
#include "src/input/input.h"
#include "src/input/event.h"
#include "src/core/output/display.h"
#include "src/output/displayBuffer.h"
#include "src/components/controllerConnector.h"
#include "src/core/output/animatedPrintable.h"

#include <memory>

namespace Slider
{
    using namespace Input;
    using namespace Output;

    class ControllerApp : public AppBase
    {
        friend class SimulatorApp;

    public:
        ControllerApp(const AppConfig& config);
        void Setup() override;
        void Update() override;

    private:
        AppConfig m_Config;
        ControllerConnector* m_Connector;
        std::unique_ptr<Display> m_Display;
        std::unique_ptr<InputReader> m_InputReader;
        DisplayBuffer m_DisplayBuffer;
        EventDispatcher m_InputDispatcher;
        AnimatedPrintable m_ConnectAnim;
    };
}

#endif
