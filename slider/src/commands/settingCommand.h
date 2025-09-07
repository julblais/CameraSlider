#ifndef SETTINGCOMMAND_H
#define SETTINGCOMMAND_H

#include "src/core/menu/menuSystem.h"

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
}

#endif
