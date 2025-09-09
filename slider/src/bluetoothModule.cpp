#include "bluetoothModule.h"

#if !IS_SIMULATOR

#include <Bluepad32.h>
#include "core/debug.h"
#include "core/address.h"

using namespace Bt;
using namespace Core;


void dumpGamepad(const ControllerPtr controller)
{
    Serial.printf(
        "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, "
        "misc: 0x%02x, gyro x:%6d y:%6d z:%6d, accel x:%6d y:%6d z:%6d\n",
        controller->index(), // Controller Index
        controller->dpad(), // D-pad
        controller->buttons(), // bitmask of pressed buttons
        controller->axisX(), // (-511 - 512) left X Axis
        controller->axisY(), // (-511 - 512) left Y axis
        controller->axisRX(), // (-511 - 512) right X axis
        controller->axisRY(), // (-511 - 512) right Y axis
        controller->brake(), // (0 - 1023): brake button
        controller->throttle(), // (0 - 1023): throttle (AKA gas) button
        controller->miscButtons(), // bitmask of pressed "misc" buttons
        controller->gyroX(), // Gyro X
        controller->gyroY(), // Gyro Y
        controller->gyroZ(), // Gyro Z
        controller->accelX(), // Accelerometer X
        controller->accelY(), // Accelerometer Y
        controller->accelZ() // Accelerometer Z
    );
}

// Only one gamepad can be connected
ControllerPtr s_Controller;

Bt::Controller::Controller(::Controller* m_controller) : m_Controller(m_controller) {}
bool Bt::Controller::IsConnected() const { return m_Controller != nullptr && m_Controller->isConnected(); }
bool Bt::Controller::HasData() const { return m_Controller != nullptr && m_Controller->hasData(); }

uint8_t Bt::Controller::Dpad() const { return m_Controller != nullptr && m_Controller->dpad(); }
bool Bt::Controller::A() const { return m_Controller != nullptr && m_Controller->a(); }
bool Bt::Controller::B() const { return m_Controller != nullptr && m_Controller->b(); }
bool Bt::Controller::X() const { return m_Controller != nullptr && m_Controller->x(); }
bool Bt::Controller::Y() const { return m_Controller != nullptr && m_Controller->y(); }
bool Bt::Controller::L1() const { return m_Controller != nullptr && m_Controller->l1(); }
bool Bt::Controller::L2() const { return m_Controller != nullptr && m_Controller->l2(); }
bool Bt::Controller::R1() const { return m_Controller != nullptr && m_Controller->r1(); }
bool Bt::Controller::R2() const { return m_Controller != nullptr && m_Controller->r2(); }
bool Bt::Controller::ThumbL() const { return m_Controller != nullptr && m_Controller->thumbL(); }
bool Bt::Controller::ThumbR() const { return m_Controller != nullptr && m_Controller->thumbR(); }
bool Bt::Controller::System() const { return m_Controller != nullptr && m_Controller->miscSystem(); }
bool Bt::Controller::Select() const { return m_Controller != nullptr && m_Controller->miscSelect(); }
bool Bt::Controller::Start() const { return m_Controller != nullptr && m_Controller->miscStart(); }

void Bt::Controller::SetPlayerLEDs(const uint8_t led) const
{
    if (m_Controller != nullptr)
        m_Controller->setPlayerLEDs(led);
}

void Bt::Controller::Rumble(const uint16_t delayedStartMs, const uint16_t durationMs, const uint8_t weakMagnitude,
                            const uint8_t strongMagnitude) const
{
    if (m_Controller != nullptr)
        m_Controller->playDualRumble(delayedStartMs, durationMs, weakMagnitude, strongMagnitude);
}

BluetoothModule& BluetoothModule::GetInstance()
{
    static BluetoothModule instance;
    return instance;
}
void BluetoothModule::Initialize()
{
    if (!m_Initialized)
    {
        BP32.setup(&onConnectedController, &onDisconnectedController);
        BP32.enableVirtualDevice(false);
        m_Initialized = true;
        LogError("Bluetooth: BluetoothModule initialized.");
    }
}

Bt::Controller BluetoothModule::GetController() const
{
    if (m_Initialized)
        return Controller(s_Controller);

    LogError("Bluetooth: Cannot get controller. BluetoothModule not initialized.");
    return Controller();
}

void BluetoothModule::DisconnectController()
{
    if (m_Initialized)
    {
        if (s_Controller != nullptr)
        {
            s_Controller->disconnect();
        }
        else
        {
            LogInfo("Bluetooth: no controller to disconnect.");
        }
    }
    else
    {
        LogError("Bluetooth: Cannot disconnect. BluetoothModule not initialized.");
    }
}

void BluetoothModule::EnablePairing()
{
    if (m_Initialized)
        BP32.enableNewBluetoothConnections(true);
    else
        LogError("Bluetooth: Cannot enable pairing. BluetoothModule not initialized.");
}

void BluetoothModule::DisablePairing()
{
    if (m_Initialized)
        BP32.enableNewBluetoothConnections(false);
    else
        LogError("Bluetooth: Cannot disable pairing. BluetoothModule not initialized.");
}

void BluetoothModule::Reset()
{
    if (m_Initialized)
        BP32.forgetBluetoothKeys();
    else
        LogError("Bluetooth: Cannot reset. BluetoothModule not initialized.");
}

MacAddress BluetoothModule::GetMacAddress() const
{
    if (m_Initialized)
        return MacAddress(BP32.localBdAddress());
    LogError("Bluetooth: Cannot get mac address. BluetoothModule not initialized.");
    return MacAddress();
}

#endif
