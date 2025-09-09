#ifndef SLIDER_BLUETOOTH_COMPONENT_H
#define SLIDER_BLUETOOTH_COMPONENT_H

#if !IS_SIMULATOR

#include "input.h"
#include "core/component.h"
#include "core/address.h"

class Controller;

namespace Bt
{
    class BluetoothComponent;

    class BluetoothGamepad : Input::InputReader
    {
        friend class BluetoothComponent;

    public:
        BluetoothGamepad(Controller* controller = nullptr);

        Input::InputData ReadInput() override;
        bool IsConnected() const;
        bool HasData() const;

        void SetPlayerLEDs(const uint8_t led) const;
        void Rumble(const uint16_t delayedStartMs, const uint16_t durationMs, const uint8_t weakMagnitude,
                    const uint8_t strongMagnitude) const;

    private:
        Input::InputData m_LastInput = Input::InputData();

        void SetController(Controller* controller) { m_Controller = controller; }
        Controller* m_Controller;
    };

    class BluetoothComponent : Core::Component
    {
    public:
        void Setup() override;
        void Update() override;

        const BluetoothGamepad* GetGamepad() const;
        void DisconnectController();
        void EnablePairing();
        void DisablePairing();
        void Reset();
        Core::MacAddress GetMacAddress() const;

    private:
        BluetoothGamepad m_Gamepad = BluetoothGamepad();
    };
}

#endif
#endif
