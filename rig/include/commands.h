#ifndef CMDS_H
#define CMDS_H

#include "utils/menuSystem.h"

class SpeedMaxCommand : public Utils::MenuCommand
{
    public:
        enum Speed  {
            SLOW = 0,
            SMED = 1,
            SHIGH = 2
        };

        SpeedMaxCommand() : m_Speed(Speed::SLOW) {}
        virtual ~SpeedMaxCommand() {}

        virtual const char* GetTitle() override;
        virtual const char* GetDesc() override;
        virtual void Invoke(Utils::MenuCommandButton command) override;

    private:
        Speed m_Speed;
};

class SpeedCurveCommand : public Utils::MenuCommand
{
    public:
        enum Curve  {
            CLINEAR = 0,
            CQUAD = 1,
            CEXP = 2
        };

        SpeedCurveCommand() :m_Curve(Curve::CLINEAR) {}
        virtual ~SpeedCurveCommand() {}

        virtual const char* GetTitle() override;
        virtual const char* GetDesc() override;
        virtual void Invoke(Utils::MenuCommandButton command) override;

    private:
        Curve m_Curve;
};

#endif