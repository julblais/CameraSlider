#ifndef CONNECTIONCOMMAND_H
#define CONNECTIONCOMMAND_H

#include "src/core/menu/menuSystem.h"
#include "src/components/brainConnector.h"
#include "src/core/output/animatedPrintable.h"
#include <memory>

using namespace Core;

namespace Slider
{
    class ConnectionCommand : public MenuCommand
    {
    public:
        ConnectionCommand(BrainConnector* connector);
        virtual void Print(Core::Display* display) const override;
        virtual void Invoke(MenuCommandButton command) override;
        virtual void OnShow() override;
        virtual void OnHide() override;
    private:
        BrainConnector* const m_Connector;
        Core::AnimatedPrintable m_AnimPrint;
    };
}

#endif