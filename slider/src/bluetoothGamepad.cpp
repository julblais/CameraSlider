#include "bluetoothComponent.h"

#if !IS_SIMULATOR

#include <Bluepad32.h>
#include "core/debug.h"
#include <cstring>
#include <string>

using namespace Bt;
using namespace Core;
using namespace IO;

#define CONTROLLER_MODEL_WII "Wii"
#define CONTROLLER_MODEL_PS4 "DualShock 4"
#define CONTROLLER_MODEL_PS5 "DualSense"
#define CONTROLLER_MODEL_SWITCH_PRO "Switch Pro"
#define CONTROLLER_MODEL_SWITCH_JOY_LEFT "Switch JoyCon Left"
#define CONTROLLER_MODEL_SWITCH_JOY_RIGHT "Switch JoyCon Right"

InputData ReadWiiInput(const Controller& controller)
{
    auto buttons = ButtonNone;
    const auto dpad = controller.dpad();
    if (dpad & DPAD_UP)
        buttons |= ButtonDpadUp;
    else if (dpad & DPAD_DOWN)
        buttons |= ButtonDpadDown;
    else if (dpad & DPAD_LEFT)
        buttons |= ButtonDpadLeft;
    else if (dpad & DPAD_RIGHT)
        buttons |= ButtonDpadRight;
    else if (controller.miscSelect())
        buttons |= ButtonSelect;
    else if (controller.miscStart())
        buttons |= ButtonCenter;
    return InputData(buttons, controller.axisRX(), controller.axisRY());
}

InputData ReadDefaultInput(const Controller& controller)
{
    auto buttons = ButtonNone;
    const auto dpad = controller.dpad();
    if (dpad & DPAD_UP)
        buttons |= ButtonDpadUp;
    else if (dpad & DPAD_DOWN)
        buttons |= ButtonDpadDown;
    else if (dpad & DPAD_LEFT)
        buttons |= ButtonDpadLeft;
    else if (dpad & DPAD_RIGHT)
        buttons |= ButtonDpadRight;
    else if (controller.miscSelect())
        buttons |= ButtonSelect;
    else if (controller.miscStart())
        buttons |= ButtonCenter;
    return InputData(buttons, controller.axisX(), controller.axisY());
}

BluetoothGamepad::BluetoothGamepad(Controller** controller)
    : m_Controller(controller) {}

InputData BluetoothGamepad::ReadInput()
{
    const auto controller = *m_Controller;
    if (controller == nullptr || !controller->hasData())
        return m_LastInput;

    //change to function pointer (no string comparison needed).
    const auto controllerName = controller->getModelName();
    if (controllerName.equals(CONTROLLER_MODEL_WII))
        ReadWiiInput(*controller);
    else if (controllerName.equals(CONTROLLER_MODEL_PS4))
        ReadDefaultInput(*controller);
    else if (controllerName.equals(CONTROLLER_MODEL_PS5))
        return ReadDefaultInput(*controller);
    else if (controllerName.equals(CONTROLLER_MODEL_SWITCH_PRO))
        return ReadDefaultInput(*controller);
    else if (controllerName.equals(CONTROLLER_MODEL_SWITCH_JOY_LEFT))
        return ReadDefaultInput(*controller);
    else if (controllerName.equals(CONTROLLER_MODEL_SWITCH_JOY_RIGHT))
        return ReadDefaultInput(*controller);
    else
        return ReadDefaultInput(*controller);
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
