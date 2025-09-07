#ifdef IS_SIMULATOR

#ifndef SIMULATOR_APP_H
#define SIMULATOR_APP_H

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

    class SimulatorApp : public AppBase
    {
    public:
        SimulatorApp(const AppConfig &config);
        void Setup() override;
        void Update() override;

    private:
        void AddDeviceMessageSimulator() const;
        AppConfig m_Config;
        std::unique_ptr<AppBase> m_BaseApp;
    };
}

#endif
#endif
