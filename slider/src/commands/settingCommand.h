#ifndef SETTINGCOMMAND_H
#define SETTINGCOMMAND_H

#include "src/core/menu/menuSystem.h"

namespace Slider
{
    class MaxSpeedCommand : public Core::MenuCommand
    {
    public:
        virtual void Print(Core::Display* display) const override;
        virtual void Invoke(Core::MenuCommandButton command) override;
    };
    
    class SpeedCurveCommand : public Core::MenuCommand
    {
    public:
        virtual void Print(Core::Display* display) const override;
        virtual void Invoke(Core::MenuCommandButton command) override;
    };
}

#endif