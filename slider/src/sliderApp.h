#ifndef SLIDERAPP_H
#define SLIDERAPP_H

#if !IS_SIMULATOR

#include "core/appBase.h"
#include "appConfig.h"
#include "event.h"
#include <memory>

namespace Core
{
    class Display;
}

namespace IO
{
    class DisplayBuffer;
    class EventDispatcher;
}

namespace Hardware
{
    class DeviceInputReader;
}

namespace Bt
{
    class BluetoothComponent;
}

namespace Slider
{
    class SliderApp : public Core::AppBase
    {
    public:
        SliderApp(const AppConfig& config);
        bool OnInputEvent(const IO::Event& inputEvent);
        void Setup() override;
        void Update() override;

    private:
        AppConfig m_Config;
        std::unique_ptr<Core::Display> m_Display;
        std::unique_ptr<Hardware::DeviceInputReader> m_LocalInput;
        std::unique_ptr<IO::DisplayBuffer> m_DisplayBuffer;
        std::unique_ptr<IO::EventDispatcher> m_InputDispatcher;
        Bt::BluetoothComponent* m_BluetoothComponent;
    };
}

#endif
#endif
