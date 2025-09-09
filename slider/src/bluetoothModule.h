#ifndef BLUETOOTHMODULE_H
#define BLUETOOTHMODULE_H

#if !IS_SIMULATOR

#include "core/address.h"

class Controller;

namespace Bt
{
    struct Gamepad
    {
        Gamepad(Controller* m_controller = nullptr);

        bool IsConnected() const;
        bool HasData() const;

        uint8_t Dpad() const;
        bool A() const;
        bool B() const;
        bool X() const;
        bool Y() const;
        bool L1() const;
        bool L2() const;
        bool R1() const;
        bool R2() const;
        bool ThumbL() const;
        bool ThumbR() const;

        bool System() const;
        bool Select() const;
        bool Start() const;

        void SetPlayerLEDs(uint8_t led) const;

        void Rumble(uint16_t delayedStartMs,
                        uint16_t durationMs,
                        uint8_t weakMagnitude,
                        uint8_t strongMagnitude) const;

    private:
        Controller* m_Controller;
    };

    class BluetoothModule
    {
    public:
        static BluetoothModule& GetInstance();

        void Initialize();

        BluetoothModule() = default;
        BluetoothModule(const BluetoothModule&) = delete;

        Gamepad GetGamepad() const;
        //void DisconnectController();
        void EnablePairing();
        void DisablePairing();
        void Reset();
        Core::MacAddress GetMacAddress() const;

    private:
        bool m_Initialized = false;
    };
}

#endif
#endif
