#ifndef CMDS_H
#define CMDS_H

#include "src/core/menu/menuSystem.h"
#include "src/core/output/display.h"
#include "settings.h"

using namespace Core;

namespace Slider
{
    class MaxSpeedCommand : public MenuCommand
    {
    public:
        virtual void Print(Core::Display* display) const override;
        virtual void Invoke(MenuCommandButton command) override;
    };

    class SpeedCurveCommand : public MenuCommand
    {
    public:
        virtual void Print(Core::Display* display) const override;
        virtual void Invoke(MenuCommandButton command) override;
    };

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

    class ControllerMacAddress : public MenuCommand
    {
    public:
        virtual void Print(Core::Display* display) const override;
        virtual void Invoke(MenuCommandButton command) override {}
    };
}

#endif