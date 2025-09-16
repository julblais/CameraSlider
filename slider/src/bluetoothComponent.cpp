#include "bluetoothComponent.h"

#if !IS_SIMULATOR

#include <Bluepad32.h>
#include "core/debug.h"

using namespace Bt;
using namespace Core;
using namespace IO;

// Only one gamepad can be connected
ControllerPtr s_Controller = nullptr;

void onConnectedController(const ControllerPtr controller)
{
    LogInfo("Bluetooth: Controller attempting to connect...");
    if (s_Controller == nullptr)
    {
        if (controller->isGamepad())
        {
            LogInfo("Bluetooth: Controller connected! model: ", controller->getModelName());
            s_Controller = controller;
        }
        else
        {
            LogError("Bluetooth: Only gamepad controllers are supported.");
        }
    }
    else
    {
        LogWarning("Bluetooth: cannot connect new controller. A controller is already connected.");
    }
}

void onDisconnectedController(const ControllerPtr ctl)
{
    if (ctl != s_Controller)
    {
        LogWarning("Bluetooth: controller disconnected, but not found in myControllers");
    }
    else
    {
        s_Controller = nullptr;
    }
}

BluetoothComponent::BluetoothComponent()
    : m_Gamepad(&s_Controller) {}

void BluetoothComponent::Setup()
{
    BP32.setup(&onConnectedController, &onDisconnectedController);
    BP32.enableNewBluetoothConnections(false);
    BP32.enableVirtualDevice(false);
    LogInfo("Bluetooth: BluetoothModule initialized.");
}

void BluetoothComponent::Update()
{
    BP32.update();
}

BluetoothGamepad* BluetoothComponent::GetGamepad()
{
    return &m_Gamepad;
}

void BluetoothComponent::DisconnectGamepad()
{
    if (s_Controller != nullptr)
    {
        s_Controller->disconnect();
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
}

void BluetoothComponent::DisablePairing()
{
    BP32.enableNewBluetoothConnections(false);
    LogInfo("Bluetooth: pairing disabled");
}

void BluetoothComponent::Reset()
{
    BP32.forgetBluetoothKeys();
    LogInfo("Bluetooth: reset connection keys");
}

MacAddress BluetoothComponent::GetMacAddress() const
{
    return MacAddress(BP32.localBdAddress());
}

#endif
