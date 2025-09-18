#ifndef SLIDERAPP_H
#define SLIDERAPP_H

#if !IS_SIMULATOR

#include "core/appBase.h"
#include "appConfig.h"
#include "event.h"
#include "core/display.h"
#include "displayBuffer.h"
#include "bluetoothComponent.h"
#include "deviceInputReader.h"
#include "eventDispatcher.h"
#include <memory>

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
        Bt::BluetoothGamepad* m_GamepadInput;
        IO::DisplayBuffer m_DisplayBuffer;
        IO::EventDispatcher m_InputDispatcher;
    };
}

#endif
#endif
