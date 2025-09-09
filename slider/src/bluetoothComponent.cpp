#include "bluetoothComponent.h"

#if !IS_SIMULATOR

#include <Bluepad32.h>
#include "core/debug.h"

using namespace Slider;
using namespace Core;

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

void onDisconnectedController(ControllerPtr ctl)
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

void BluetoothComponent::Setup()
{
    BP32.setup(&onConnectedController, &onDisconnectedController);


    // Enables mouse / touchpad support for gamepads that support them.
    // When enabled, controllers like DualSense and DualShock4 generate two connected devices:
    // - First one: the gamepad
    // - Second one, which is a "virtual device", is a mouse.
    // By default, it is disabled.
    BP32.enableVirtualDevice(false);
}


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

void BluetoothComponent::Update()
{
    // This call fetches all the controllers' data.
    // Call this function in your main loop.
    bool dataUpdated = BP32.update();
    if (!dataUpdated)
        return;

    for (const auto myController : s_Controllers)
    {
        if (myController && myController->isConnected() && myController->hasData() && myController->isGamepad())
            processGamepad(myController);
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

void BluetoothComponent::ResetConnections()
{
    BP32.forgetBluetoothKeys();
}

MacAddress BluetoothComponent::GetMacAddress() const
{
    return MacAddress(BP32.localBdAddress());
}

#endif
