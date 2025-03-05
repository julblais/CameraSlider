#ifndef CONNECTIONCOMMAND_H
#define CONNECTIONCOMMAND_H

#include "src/core/menu/menuSystem.h"
#include "src/components/brainConnector.h"
#include <memory>

using namespace Core;

namespace Slider
{
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