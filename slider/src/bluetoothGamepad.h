#ifndef BLUETOOTHGAMEPAD_H
#define BLUETOOTHGAMEPAD_H

#if !IS_SIMULATOR

#include <cstdint>
#include "event.h"
#include <memory>

class Controller;

namespace Bt
{
    class BluetoothComponent;

    class BluetoothGamepad
    {
    public:
        enum class Model
        {
            Generic,
            Wii,
            PS4,
            PS5,
            SwitchPro,
            SwitchJoyLeft,
            SwitchJoyRight
        };

        BluetoothGamepad(Controller* controller = nullptr);

        IO::InputData ReadInput();
        bool IsConnected() const;
        bool HasData() const;
        Controller* GetController() const;
        Model GetModel() const;

        std::unique_ptr<char[]> GetDescription() const;

        void SetPlayerLEDs(const uint8_t led) const;
        void Rumble(const uint16_t delayedStartMs, const uint16_t durationMs, const uint8_t weakMagnitude,
                    const uint8_t strongMagnitude) const;

    private:
        friend class BluetoothComponent;
        IO::InputData m_LastInput;
        Controller* m_Controller;
        Model m_Model;
    };
}

#endif
#endif
