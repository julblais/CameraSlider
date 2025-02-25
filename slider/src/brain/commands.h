#ifndef CMDS_H
#define CMDS_H

#include "src/core/menu/menuSystem.h"
#include "settings.h"

using namespace Core;

namespace Net
{
    class WifiModule;
}

namespace Slider
{
    class MaxSpeedCommand : public MenuCommand
    {
    public:
        virtual const char* GetTitle() override;
        virtual const char* GetDesc() override;
        virtual void Invoke(MenuCommandButton command) override;
    };

    class SpeedCurveCommand : public MenuCommand
    {
    public:
        virtual const char* GetTitle() override;
        virtual const char* GetDesc() override;
        virtual void Invoke(MenuCommandButton command) override;
    };

    class BrainMacAddress : public MenuCommand
    {
    public:
        BrainMacAddress(Net::WifiModule* wifiModule);
        virtual const char* GetTitle() override;
        virtual const char* GetDesc() override;
        virtual bool ShowArrows() { return false; }
        virtual void Invoke(MenuCommandButton command) override;
    private:
        Net::WifiModule* m_Wifi;
    };

    class ControllerMacAddress : public MenuCommand
    {
    public:
        virtual const char* GetTitle() override;
        virtual const char* GetDesc() override;
        virtual bool ShowArrows() { return false; }
        virtual void Invoke(MenuCommandButton command) override;
    };
}

#endif