#ifndef CONNECTIONCOMMAND_H
#define CONNECTIONCOMMAND_H

#include "core/menuSystem.h"
#include "brainConnector.h"
#include "core/animatedPrintable.h"

using namespace Core;

namespace Slider
{
    class ConnectionCommand : public MenuCommand
    {
    public:
        ConnectionCommand(BrainConnector* connector);
        void Print(Display* display) const override;
        void Invoke(MenuCommandButton command) override;
        void OnShow() override;
        void OnHide() override;

    private:
        BrainConnector* const m_Connector;
        AnimatedPrintable m_AnimPrint;
    };
}

#endif
