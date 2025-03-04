#ifndef CONTROLLERAPP_H
#define CONTROLLERAPP_H

#include "src/core/app/appBase.h"
#include "src/app/appConfig.h"
#include "src/input/input.h"
#include "src/input/event.h"
#include "src/core/output/display.h"
#include "src/output/displayBuffer.h"

#include <memory>

namespace Slider
{
    using namespace Input;
    using namespace Output;

    class ControllerApp : public Core::AppBase
    {
    public:
        ControllerApp(const AppConfig& config);
        virtual void Setup() override;
        virtual void Update() override;

    private:
        AppConfig m_Config;
        std::unique_ptr<Core::Display> m_Display;
        std::unique_ptr<Input::InputReader> m_InputReader;
        DisplayBuffer m_DisplayBuffer;
        EventDispatcher m_InputDispatcher;
    };
}

#endif