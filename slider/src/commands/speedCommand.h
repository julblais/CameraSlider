#ifndef SPEEDCOMMAND_H
#define SPEEDCOMMAND_H

#include "src/core/menu/menuSystem.h"

namespace Slider
{
    class MaxSpeedCommand : public Core::MenuCommand
    {
    public:
        virtual void Print(Core::Display* display) const override;
        virtual void Invoke(Core::MenuCommandButton command) override;
    };
}

#endif