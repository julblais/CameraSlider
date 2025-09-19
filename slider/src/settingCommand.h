#ifndef SETTINGCOMMAND_H
#define SETTINGCOMMAND_H

#include "core/menuSystem.h"

namespace Bt
{
    class BluetoothComponent;
}

namespace Slider
{
    class MaxSpeedCommand : public Core::MenuCommand
    {
    public:
        void Print(Core::Display* display) const override;
        void Invoke(const Core::MenuCommandButton command) override;
    };

    class SpeedCurveCommand : public Core::MenuCommand
    {
    public:
        void Print(Core::Display* display) const override;
        void Invoke(const Core::MenuCommandButton command) override;
    };

    class GamepadNameCommand : public Core::MenuCommand
    {
    public:
        GamepadNameCommand(Bt::BluetoothComponent* bluetooth);

        void Print(Core::Display* display) const override;
        void Invoke(const Core::MenuCommandButton command) override;

    private:
        Bt::BluetoothComponent* m_Bluetooth;
    };
}

#endif
