#include "bluetoothGamepad.h"

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

BluetoothGamepad::Model GetGamepadModel(const Controller* controller)
{
    if (controller == nullptr)
        return BluetoothGamepad::Model::Generic;

    const auto modelName = controller->getModelName();
    if (modelName.equals(CONTROLLER_MODEL_WII))
        return BluetoothGamepad::Model::Wii;
    if (modelName.equals(CONTROLLER_MODEL_PS4))
        return BluetoothGamepad::Model::PS4;
    if (modelName.equals(CONTROLLER_MODEL_PS5))
        return BluetoothGamepad::Model::PS5;
    if (modelName.equals(CONTROLLER_MODEL_SWITCH_PRO))
        return BluetoothGamepad::Model::SwitchPro;
    if (modelName.equals(CONTROLLER_MODEL_SWITCH_JOY_LEFT))
        return BluetoothGamepad::Model::SwitchJoyLeft;
    if (modelName.equals(CONTROLLER_MODEL_SWITCH_JOY_RIGHT))
        return BluetoothGamepad::Model::SwitchJoyRight;
    return BluetoothGamepad::Model::Generic;
}

InputData ReadWiiInput(const Controller& controller)
{
    auto buttons = ButtonNone;
    const auto dpad = controller.dpad();
    if (dpad & DPAD_UP)
        buttons |= ButtonDpadUp;
    if (dpad & DPAD_DOWN)
        buttons |= ButtonDpadDown;
    if (dpad & DPAD_LEFT)
        buttons |= ButtonDpadLeft;
    if (dpad & DPAD_RIGHT)
        buttons |= ButtonDpadRight;
    if (controller.miscSelect())
        buttons |= ButtonSelect;
    if (controller.miscStart())
        buttons |= ButtonCenter;
    return InputData(buttons, controller.axisRX(), controller.axisRY());
}

InputData ReadDefaultInput(const Controller& controller)
{
    auto buttons = ButtonNone;
    const auto dpad = controller.dpad();
    if (dpad & DPAD_UP)
        buttons |= ButtonDpadUp;
    if (dpad & DPAD_DOWN)
        buttons |= ButtonDpadDown;
    if (dpad & DPAD_LEFT)
        buttons |= ButtonDpadLeft;
    if (dpad & DPAD_RIGHT)
        buttons |= ButtonDpadRight;
    if (controller.miscSelect())
        buttons |= ButtonSelect;
    if (controller.miscStart())
        buttons |= ButtonCenter;
    return InputData(buttons, controller.axisX(), controller.axisY());
}

BluetoothGamepad::BluetoothGamepad(Controller* controller)
    : m_Controller(controller),
      m_Model(GetGamepadModel(controller)) {}

InputData BluetoothGamepad::ReadInput()
{
    if (m_Controller != nullptr && m_Controller->hasData())
    {
        switch (m_Model)
        {
        case Model::Wii:
            m_LastInput = ReadWiiInput(*m_Controller);
            break;
        case Model::PS4:
        case Model::PS5:
        case Model::SwitchPro:
        case Model::SwitchJoyLeft:
        case Model::SwitchJoyRight:
            m_LastInput = ReadDefaultInput(*m_Controller);
            break;
        default:
            m_LastInput = ReadDefaultInput(*m_Controller);
            break;
        }
    }

    return m_LastInput;
}

bool BluetoothGamepad::IsConnected() const
{
    return m_Controller != nullptr && m_Controller->isConnected();
}

bool BluetoothGamepad::HasData() const
{
    return m_Controller != nullptr && m_Controller->hasData();
}

std::unique_ptr<char[]> BluetoothGamepad::GetDescription() const
{
    if (m_Controller != nullptr)
    {
        const auto modelName = m_Controller->getModelName();
        std::unique_ptr<char[]> buffer(new char[modelName.length() + 1]);
        std::strcpy(buffer.get(), modelName.c_str());
        return buffer;
    }
    return nullptr;
}

void BluetoothGamepad::SetPlayerLEDs(const uint8_t led) const
{
    if (m_Controller != nullptr)
        m_Controller->setPlayerLEDs(led);
    else
        LogWarning("Bluetooth: cannot set led. A controller is not connected.");
}

void BluetoothGamepad::Rumble(const uint16_t delayedStartMs, const uint16_t durationMs, const uint8_t weakMagnitude,
                              const uint8_t strongMagnitude) const
{
    if (m_Controller != nullptr)
        m_Controller->playDualRumble(delayedStartMs, durationMs, weakMagnitude, strongMagnitude);
    else
        LogWarning("Bluetooth: cannot set rumble. A controller is not connected.");
}

Controller* BluetoothGamepad::GetController() const
{
    return m_Controller;
}

BluetoothGamepad::Model BluetoothGamepad::GetModel() const
{
    return m_Model;
}

#endif
