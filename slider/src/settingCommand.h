#ifndef SETTINGCOMMAND_H
#define SETTINGCOMMAND_H

#include "core/menuSystem.h"

namespace Slider
{
    class MaxSpeedCommand : public Core::MenuCommand
    {
    public:
        void Print(Core::Display& display) const override;
        void Invoke(const Button command) override;
    };

    class SpeedCurveCommand : public Core::MenuCommand
    {
    public:
        void Print(Core::Display& display) const override;
        void Invoke(const Button command) override;
    };
}

#endif
