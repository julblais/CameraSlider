#ifndef BLUETOOTH_GAMEPAD_H
#define BLUETOOTH_GAMEPAD_H

#if !IS_SIMULATOR

#include <cstdint>
#include "input.h"

class Controller;

namespace Bt
{
    class BluetoothComponent;

    class BluetoothGamepad : IO::InputReader
    {
    public:
        IO::InputData ReadInput() override;
        bool IsConnected() const;
        bool HasData() const;

        void SetPlayerLEDs(const uint8_t led) const;
        void Rumble(const uint16_t delayedStartMs, const uint16_t durationMs, const uint8_t weakMagnitude,
                    const uint8_t strongMagnitude) const;

    private:
        friend class BluetoothComponent;
        BluetoothGamepad();
        IO::InputData m_LastInput = IO::InputData();
    };
}

#endif
#endif
