#ifndef BLUETOOTHCOMMANDS_H
#define BLUETOOTHCOMMANDS_H

#if !IS_SIMULATOR

#include "core/menuSystem.h"

namespace Bt
{
    class BluetoothComponent;

    class GamepadNameCommand : public Core::MenuCommand
    {
    public:
        GamepadNameCommand(Bt::BluetoothComponent* bluetooth);

        void Print(Core::Display* display) const override;
        void Invoke(const Core::MenuCommandButton command) override;

    private:
        BluetoothComponent* m_Bluetooth;
    };
}

#endif
#endif
