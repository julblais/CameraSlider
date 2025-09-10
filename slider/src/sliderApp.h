#ifndef SLIDERAPP_H
#define SLIDERAPP_H

#include "core/appBase.h"
#include "appConfig.h"
#include "event.h"
#include "core/display.h"
#include "displayBuffer.h"
#include "bluetoothComponent.h"

#include <memory>

#include "deviceInputReader.h"

namespace Slider
{
    class SliderApp : public AppBase
    {
    public:
        SliderApp(const AppConfig& config);
        void Setup() override;
        void Update() override;

    private:
        AppConfig m_Config;
        std::unique_ptr<Display> m_Display;
        std::unique_ptr<Hardware::DeviceInputReader> m_LocalInput;
        Bt::BluetoothGamepad* m_GamepadInput;
        IO::DisplayBuffer m_DisplayBuffer;
        IO::EventDispatcher m_InputDispatcher;
    };
}

#endif
