#include "bluetoothComponent.h"

#if !IS_SIMULATOR

#include <Bluepad32.h>
#include "core/debug.h"
#include "bluetoothGamepad.h"

using namespace Bt;
using namespace Core;

// Only one gamepad can be connected
static BluetoothGamepad s_Gamepad{};

void onConnectedController(const ControllerPtr controller)
{
    LogInfo("Bluetooth: Controller attempting to connect...");
    if (!s_Gamepad.IsConnected())
    {
        LogInfo("Bluetooth: Controller connected! model: ", controller->getModelName(), ", is gamepad: ",
                controller->isGamepad());
        s_Gamepad = BluetoothGamepad(controller);
    }
    else
    {
        LogWarning("Bluetooth: cannot connect new controller. A controller is already connected.");
    }
}

void onDisconnectedController(const ControllerPtr ctl)
{
    if (s_Gamepad.GetController() != ctl)
    {
        LogWarning("Bluetooth: controller disconnected, but not found in myControllers");
    }
    else
    {
        s_Gamepad = BluetoothGamepad();
    }
}

void BluetoothComponent::Setup()
{
    BP32.setup(&onConnectedController, &onDisconnectedController);
    BP32.enableNewBluetoothConnections(false);
    LogInfo("Bluetooth: BluetoothModule initialized.");
}

void BluetoothComponent::Update()
{
    BP32.update();
}

BluetoothGamepad* BluetoothComponent::GetGamepad()
{
    return &s_Gamepad;
}

void BluetoothComponent::DisconnectGamepad()
{
    if (s_Gamepad.IsConnected())
    {
        s_Gamepad.GetController()->disconnect();
    }
    else
    {
        LogWarning("Bluetooth: no controller to disconnect.");
    }
}

void BluetoothComponent::EnablePairing()
{
    BP32.enableNewBluetoothConnections(true);
    LogInfo("Bluetooth: pairing enabled");
    m_IsPairing = true;
}

void BluetoothComponent::DisablePairing()
{
    BP32.enableNewBluetoothConnections(false);
    LogInfo("Bluetooth: pairing disabled");
    m_IsPairing = false;
}

bool BluetoothComponent::IsPairing() const
{
    return m_IsPairing;
}

void BluetoothComponent::Reset()
{
    BP32.forgetBluetoothKeys();
    LogInfo("Bluetooth: reset connection keys");
    DisablePairing();
}

MacAddress BluetoothComponent::GetMacAddress() const
{
    return MacAddress(BP32.localBdAddress());
}

#endif
