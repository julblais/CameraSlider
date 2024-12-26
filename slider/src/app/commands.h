#ifndef CMDS_H
#define CMDS_H

#include "src/utils/menuSystem.h"
#include "src/app/settings.h"

namespace Slider
{
    class MaxSpeedCommand : public Utils::MenuCommand
    {
        public:
            MaxSpeedCommand() = default;
            virtual ~MaxSpeedCommand() = default;

            virtual const char* GetTitle() override;
            virtual const char* GetDesc() override;
            virtual void Invoke(Utils::MenuCommandButton command) override;
    };

    class SpeedCurveCommand : public Utils::MenuCommand
    {
        public:
            SpeedCurveCommand() = default;
            virtual ~SpeedCurveCommand() = default;

            virtual const char* GetTitle() override;
            virtual const char* GetDesc() override;
            virtual void Invoke(Utils::MenuCommandButton command) override;
    };
}


#endif