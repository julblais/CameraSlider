#ifndef SLIDER_BLUETOOTH_COMPONENT_H
#define SLIDER_BLUETOOTH_COMPONENT_H

#if !IS_SIMULATOR

#include <ArduinoController.h>

#include "input.h"
#include "core/component.h"
#include "core/address.h"

class Controller;

namespace Bt
{
    class BluetoothComponent;

    class BluetoothGamepad
    {
    public:
        IO::InputData ReadInput();
        bool IsConnected() const;
        bool HasData() const;

        void SetPlayerLEDs(const uint8_t led);
        void Rumble(const uint16_t delayedStartMs, const uint16_t durationMs, const uint8_t weakMagnitude,
                    const uint8_t strongMagnitude);

    private:
        friend class BluetoothComponent;
        BluetoothGamepad(Controller** controller);
        IO::InputData m_LastInput = IO::InputData();
        Controller** m_Controller;
    };

    class BluetoothComponent : public Core::Component
    {
    public:
        BluetoothComponent();
        void Setup() override;
        void Update() override;

        BluetoothGamepad* GetGamepad();
        void DisconnectGamepad();
        void EnablePairing();
        void DisablePairing();
        void Reset();
        Core::MacAddress GetMacAddress() const;

    private:
        BluetoothGamepad m_Gamepad;
    };
}

#endif
#endif
