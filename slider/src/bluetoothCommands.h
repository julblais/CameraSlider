#ifndef BLUETOOTHCOMMANDS_H
#define BLUETOOTHCOMMANDS_H

#if !IS_SIMULATOR

#include "core/animatedPrintable.h"
#include "core/menuSystem.h"
#include "core/timer.h"

namespace Bt
{
    class BluetoothComponent;

    class GamepadNameCommand : public Core::MenuCommand
    {
    public:
        GamepadNameCommand(BluetoothComponent* bluetooth);

        void Print(Core::Display* display) const override;
        void Invoke(const Button command) override {}

    private:
        BluetoothComponent* m_Bluetooth;
    };

    class GamepadConnectionCommand : public Core::MenuCommand
    {
    public:
        GamepadConnectionCommand(BluetoothComponent* bluetooth);

        void Print(Core::Display* display) const override;
        void Invoke(const Button command) override;

        void OnUpdate() override;
        void OnHide() override;

    private:
        BluetoothComponent* m_Bluetooth;
        bool m_IsPairing;
        Core::AnimatedPrintable m_Progress;
        bool m_ShowConnectionResult;
        Core::Timer m_ConnectionResultTimer;
    };
}

#endif
#endif
