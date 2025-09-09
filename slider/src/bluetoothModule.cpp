#include "bluetoothModule.h"

#if !IS_SIMULATOR

#include <Bluepad32.h>
#include "core/debug.h"
#include "core/address.h"

using namespace Bt;
using namespace Core;

Bt::Gamepad::Gamepad(Controller* m_controller) : m_Controller(m_controller) {}
bool Bt::Gamepad::IsConnected() const { return m_Controller != nullptr && m_Controller->isConnected(); }
bool Bt::Gamepad::HasData() const { return m_Controller != nullptr && m_Controller->hasData(); }

uint8_t Bt::Gamepad::Dpad() const { return m_Controller != nullptr && m_Controller->dpad(); }
bool Bt::Gamepad::A() const { return m_Controller != nullptr && m_Controller->a(); }
bool Bt::Gamepad::B() const { return m_Controller != nullptr && m_Controller->b(); }
bool Bt::Gamepad::X() const { return m_Controller != nullptr && m_Controller->x(); }
bool Bt::Gamepad::Y() const { return m_Controller != nullptr && m_Controller->y(); }
bool Bt::Gamepad::L1() const { return m_Controller != nullptr && m_Controller->l1(); }
bool Bt::Gamepad::L2() const { return m_Controller != nullptr && m_Controller->l2(); }
bool Bt::Gamepad::R1() const { return m_Controller != nullptr && m_Controller->r1(); }
bool Bt::Gamepad::R2() const { return m_Controller != nullptr && m_Controller->r2(); }
bool Bt::Gamepad::ThumbL() const { return m_Controller != nullptr && m_Controller->thumbL(); }
bool Bt::Gamepad::ThumbR() const { return m_Controller != nullptr && m_Controller->thumbR(); }
bool Bt::Gamepad::System() const { return m_Controller != nullptr && m_Controller->miscSystem(); }
bool Bt::Gamepad::Select() const { return m_Controller != nullptr && m_Controller->miscSelect(); }
bool Bt::Gamepad::Start() const { return m_Controller != nullptr && m_Controller->miscStart(); }

void Bt::Gamepad::SetPlayerLEDs(uint8_t led) const
{
    if (m_Controller != nullptr)
        m_Controller->setPlayerLEDs(led);
}

void Bt::Gamepad::Rumble(uint16_t delayedStartMs, uint16_t durationMs, uint8_t weakMagnitude,
                         uint8_t strongMagnitude) const
{
    if (m_Controller != nullptr)
        m_Controller->playDualRumble(delayedStartMs, durationMs, weakMagnitude, strongMagnitude);
}

BluetoothModule& BluetoothModule::GetInstance()
{
    static BluetoothModule instance;
    return instance;
}

// Up to 4 gamepads can be connected at the same time.
ControllerPtr s_Controllers[BP32_MAX_GAMEPADS];

void onConnectedController(const ControllerPtr controller)
{
    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++)
    {
        if (!controller->isGamepad())
        {
            LogError("Only gamepad controllers are supported.");
        }
        else if (s_Controllers[i] == nullptr)
        {
            LogInfo("Bluetooth: Controller is connected, index: ", i, " model: ", controller->getModelName());
            foundEmptySlot = true;
            break;
        }
    }
    if (!foundEmptySlot)
    {
        LogWarning("Bluetooth: controller found, but could not find empty slot");
    }
}

void onDisconnectedController(const ControllerPtr ctl)
{
    bool foundController = false;

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++)
    {
        if (s_Controllers[i] == ctl)
        {
            LogInfo("Bluetooth: controller disconnected from index=", i);
            s_Controllers[i] = nullptr;
            foundController = true;
            break;
        }
    }

    if (!foundController)
    {
        LogWarning("Bluetooth: controller disconnected, but not found in myControllers");
    }
}

void BluetoothModule::Initialize()
{
    BP32.setup(&onConnectedController, &onDisconnectedController);
    BP32.enableVirtualDevice(false);
    m_Initialized = true;
}

Bt::Gamepad BluetoothModule::GetGamepad() const
{
    if (m_Initialized)
    {
        
    }
    return Gamepad();
}

void BluetoothModule::EnablePairing()
{
    if (m_Initialized)
        BP32.enableNewBluetoothConnections(true);
    else
        LogError("Cannot enable pairing. BluetoothModule not initialized.");
}

void BluetoothModule::DisablePairing()
{
    if (m_Initialized)
        BP32.enableNewBluetoothConnections(false);
    else
        LogError("Cannot disable pairing. BluetoothModule not initialized.");
}

void BluetoothModule::Reset()
{
    if (m_Initialized)
        BP32.forgetBluetoothKeys();
    else
        LogError("Cannot reset. BluetoothModule not initialized.");
}

MacAddress BluetoothModule::GetMacAddress() const
{
    if (m_Initialized)
        return MacAddress(BP32.localBdAddress());
    LogError("Cannot get mac address. BluetoothModule not initialized.");
    return MacAddress();
}

#endif
