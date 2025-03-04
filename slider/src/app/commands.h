#ifndef CMDS_H
#define CMDS_H

#include "src/core/menu/menuSystem.h"
#include "src/core/output/display.h"
#include "src/app/brain/connectApp.h"
#include <memory>
#include "settings.h"

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

    class ConnectionCommand : public MenuCommand
    {
        enum class State
        {
            NotConnected,
            Connected,
            Connecting
        };
    public:
        virtual void Print(Core::Display* display) const override;
        virtual void Invoke(MenuCommandButton command) override;
        virtual void OnUpdate() override;
        virtual void OnShow() override;
        virtual void OnHide() override;
    private:
        std::unique_ptr<Slider::BrainConnector> m_Connector;
        State m_State;
    };
}

#endif