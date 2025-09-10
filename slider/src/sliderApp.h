#ifndef SLIDERAPP_H
#define SLIDERAPP_H

#include "core/appBase.h"
#include "appConfig.h"
#include "input.h"
#include "event.h"
#include "core/display.h"
#include "displayBuffer.h"
#include "bluetoothComponent.h"

#include <memory>

namespace Slider
{
    using namespace IO;

    class SliderApp : public AppBase
    {
    public:
        SliderApp(const AppConfig& config);
        void Setup() override;
        void Update() override;

    private:
        AppConfig m_Config;
        std::unique_ptr<Display> m_Display;
        std::unique_ptr<InputReader> m_LocalInputReader;

        Bt::BluetoothGamepad* m_Gamepad;
        DisplayBuffer m_DisplayBuffer;
        EventDispatcher m_InputDispatcher;
    };
}

#endif
