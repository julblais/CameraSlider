#ifndef ADDRESSCOMMAND_H
#define ADDRESSCOMMAND_H

#include "src/core/menu/menuSystem.h"
#include <memory>

using namespace Core;

namespace Slider
{
    class BrainAddressCommand : public MenuCommand
    {
    public:
        virtual void Print(Core::Display* display) const override;
        virtual void Invoke(MenuCommandButton command) override {}
    };

    class ControllerAddressCommand : public MenuCommand
    {
    public:
        virtual void Print(Core::Display* display) const override;
        virtual void Invoke(MenuCommandButton command) override {}
    };
}

#endif