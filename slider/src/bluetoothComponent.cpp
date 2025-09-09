#include "bluetoothComponent.h"

#if !IS_SIMULATOR

#include <Bluepad32.h>
#include "bluetoothGamepad.h"
#include "core/debug.h"

using namespace Bt;
using namespace Core;

// Only one gamepad can be connected
ControllerPtr s_Controller = nullptr;

BluetoothGamepad::BluetoothGamepad() {}

IO::InputData BluetoothGamepad::ReadInput()
{
    if (s_Controller != nullptr && s_Controller->hasData())
    {
        auto buttons = IO::ButtonEvent::None;
        const auto dpad = s_Controller->dpad();
        if (dpad & DPAD_UP)
            buttons = IO::ButtonEvent::Up;
        else if (dpad & DPAD_DOWN)
            buttons = IO::ButtonEvent::Down;
        else if (dpad & DPAD_LEFT)
            buttons = IO::ButtonEvent::Left;
        else if (dpad & DPAD_RIGHT)
            buttons = IO::ButtonEvent::Right;
        else if (s_Controller->miscSelect())
            buttons = IO::ButtonEvent::Select;
        else if (s_Controller->miscStart())
            buttons = IO::ButtonEvent::Center;
        m_LastInput = IO::InputData(buttons);
    }
    return m_LastInput;
}

bool BluetoothGamepad::IsConnected() const
{
    return s_Controller != nullptr && s_Controller->isConnected();
}

bool BluetoothGamepad::HasData() const
{
    return s_Controller != nullptr && s_Controller->hasData();
}

void BluetoothGamepad::SetPlayerLEDs(const uint8_t led) const
{
    if (s_Controller != nullptr)
        s_Controller->setPlayerLEDs(led);
    else
        LogWarning("Bluetooth: cannot set led. A controller is not connected.");
}

void BluetoothGamepad::Rumble(const uint16_t delayedStartMs, const uint16_t durationMs, const uint8_t weakMagnitude,
                              const uint8_t strongMagnitude) const
{
    if (s_Controller != nullptr)
        s_Controller->playDualRumble(delayedStartMs, durationMs, weakMagnitude, strongMagnitude);
    else
        LogWarning("Bluetooth: cannot set rumble. A controller is not connected.");
}

void onConnectedController(const ControllerPtr controller)
{
    LogInfo("Bluetooth: Controller attempting to connect...");
    if (s_Controller != nullptr)
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

void BluetoothComponent::Setup()
{
    BP32.setup(&onConnectedController, &onDisconnectedController);
    BP32.enableVirtualDevice(false);
    LogError("Bluetooth: BluetoothModule initialized.");
}

void BluetoothComponent::Update()
{
    BP32.update();
}

BluetoothGamepad BluetoothComponent::GetGamepad() const
{
    return BluetoothGamepad();
}

void BluetoothComponent::DisconnectController()
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

void BluetoothComponent::EnablePairing()
{
    BP32.enableNewBluetoothConnections(true);
}

void BluetoothComponent::DisablePairing()
{
    BP32.enableNewBluetoothConnections(false);
}

void BluetoothComponent::Reset()
{
    BP32.forgetBluetoothKeys();
}

MacAddress BluetoothComponent::GetMacAddress() const
{
    return MacAddress(BP32.localBdAddress());
}

/*
void processGamepad(ControllerPtr ctl)
{
    // There are different ways to query whether a button is pressed.
    // By query each button individually:
    //  a(), b(), x(), y(), l1(), etc...
    if (ctl->a())
    {
        static int colorIdx = 0;
        // Some gamepads like DS4 and DualSense support changing the color LED.
        // It is possible to change it by calling:
        switch (colorIdx % 3)
        {
        case 0:
            // Red
            ctl->setColorLED(255, 0, 0);
            break;
        case 1:
            // Green
            ctl->setColorLED(0, 255, 0);
            break;
        case 2:
            // Blue
            ctl->setColorLED(0, 0, 255);
            break;
        }
        colorIdx++;
    }

    if (ctl->b())
    {
        // Turn on the 4 LED. Each bit represents one LED.
        static int led = 0;
        led++;
        // Some gamepads like the DS3, DualSense, Nintendo Wii, Nintendo Switch
        // support changing the "Player LEDs": those 4 LEDs that usually indicate
        // the "gamepad seat".
        // It is possible to change them by calling:
        ctl->setPlayerLEDs(led & 0x0f);
    }

    if (ctl->x())
    {
        // Some gamepads like DS3, DS4, DualSense, Switch, Xbox One S, Stadia support rumble.
        // It is possible to set it by calling:
        // Some controllers have two motors: "strong motor", "weak motor".
        // It is possible to control them independently.
        ctl->playDualRumble(0, 250, 0x80,
                            0x40);
    }

    // Another way to query controller data is by getting the buttons() function.
    // See how the different "dump*" functions dump the Controller info.
    dumpGamepad(ctl);
}
*/


#endif
