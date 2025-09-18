#include "bluetoothComponent.h"

#if !IS_SIMULATOR

#include "core/utils/enumUtils.h"
#include <Bluepad32.h>
#include "core/debug.h"
#include <cstring>
#include <string>

using namespace Bt;
using namespace Core;
using namespace IO;
using namespace Core::Enums;

BluetoothGamepad::BluetoothGamepad(Controller** controller)
    : m_Controller(controller) {}

InputData BluetoothGamepad::ReadInput()
{
    const auto controller = *m_Controller;
    if (controller != nullptr && controller->hasData())
    {
        auto buttons = ButtonEvent::None;
        const auto dpad = controller->dpad();
        if (dpad & DPAD_UP)
            buttons |= ButtonEvent::Up;
        else if (dpad & DPAD_DOWN)
            buttons |= ButtonEvent::Down;
        else if (dpad & DPAD_LEFT)
            buttons |= ButtonEvent::Left;
        else if (dpad & DPAD_RIGHT)
            buttons |= ButtonEvent::Right;
        else if (controller->miscSelect())
            buttons |= ButtonEvent::Select;
        else if (controller->miscStart())
            buttons |= ButtonEvent::Center;
        m_LastInput = InputData(buttons, controller->axisRX(), controller->axisRY());
    }
    return m_LastInput;
}

bool BluetoothGamepad::IsConnected() const
{
    const auto controller = *m_Controller;
    return controller != nullptr && controller->isConnected();
}

bool BluetoothGamepad::HasData() const
{
    const auto controller = *m_Controller;
    return controller != nullptr && controller->hasData();
}

std::unique_ptr<char[]> BluetoothGamepad::GetDescription() const
{
    const auto controller = *m_Controller;

    String modelName = "";
    if (controller != nullptr)
        modelName = controller->getModelName();

    std::unique_ptr<char[]> buffer(new char[modelName.length() + 1]);
    std::strcpy(buffer.get(), modelName.c_str());
    return buffer;
}

void BluetoothGamepad::SetPlayerLEDs(const uint8_t led)
{
    const auto controller = *m_Controller;
    if (controller != nullptr)
        controller->setPlayerLEDs(led);
    else
        LogWarning("Bluetooth: cannot set led. A controller is not connected.");
}

void BluetoothGamepad::Rumble(const uint16_t delayedStartMs, const uint16_t durationMs, const uint8_t weakMagnitude,
                              const uint8_t strongMagnitude)
{
    const auto controller = *m_Controller;
    if (controller != nullptr)
        controller->playDualRumble(delayedStartMs, durationMs, weakMagnitude, strongMagnitude);
    else
        LogWarning("Bluetooth: cannot set rumble. A controller is not connected.");
}

#endif
