#ifndef ADDRESSCOMMAND_H
#define ADDRESSCOMMAND_H

#include "core/menuSystem.h"

using namespace Core;

namespace Slider
{
    class BrainAddressCommand : public MenuCommand
    {
    public:
        void Print(Display* display) const override;
        void Invoke(MenuCommandButton command) override {}
    };

    class ControllerAddressCommand : public MenuCommand
    {
    public:
        void Print(Display* display) const override;
        void Invoke(MenuCommandButton command) override {}
    };
}

#endif
