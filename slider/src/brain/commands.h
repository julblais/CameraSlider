#ifndef CMDS_H
#define CMDS_H

#include "src/core/menu/menuSystem.h"
#include "settings.h"

using namespace Core;

namespace Slider
{
    class MaxSpeedCommand : public MenuCommand
    {
    public:
        MaxSpeedCommand() = default;
        virtual ~MaxSpeedCommand() = default;

        virtual const char* GetTitle() override;
        virtual const char* GetDesc() override;
        virtual void Invoke(MenuCommandButton command) override;
    };

    class SpeedCurveCommand : public MenuCommand
    {
    public:
        SpeedCurveCommand() = default;
        virtual ~SpeedCurveCommand() = default;

        virtual const char* GetTitle() override;
        virtual const char* GetDesc() override;
        virtual void Invoke(MenuCommandButton command) override;
    };
}

#endif