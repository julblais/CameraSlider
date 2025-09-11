#ifndef BLUETOOTHGAMEPAD_H
#define BLUETOOTHGAMEPAD_H

#if !IS_SIMULATOR

#include <cstdint>
#include "input.h"

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
}

#endif
#endif
