#include "connectionCommand.h"
#include "settings.h"
#include "src/network/wifiModule.h"

using namespace Core;
using namespace Slider;

ConnectionCommand::ConnectionCommand(Slider::BrainConnector* connector)
    :m_Connector(connector), m_AnimPrint("", { " ", ".", "..", "..." })
{}

void ConnectionCommand::Print(Display* display) const
{
    PrintTitle(display, "Manette");
    auto state = m_Connector->GetState();
    if (state == BrainConnector::State::CONNECTED)
        display->PrintLine(1, "  Connectee!");
    else if (state == BrainConnector::State::IDLE)
    {
        const auto rightArrow = display->GetSymbol(Symbol::RightArrow);
        display->PrintLine(1, " ", rightArrow, "Commencer");
    }
    else
        display->PrintLine(1, "  Connexion", m_AnimPrint);
}

void ConnectionCommand::Invoke(MenuCommandButton command)
{
    if (m_Connector->GetState() != BrainConnector::State::CONNECTED)
    {
        if (command == MenuCommandButton::SELECT)
        {
            m_Connector->BeginConnectionAttempt();
            m_AnimPrint.Start(500);
        }
    }
}

void ConnectionCommand::OnShow()
{}

void ConnectionCommand::OnHide()
{
    m_AnimPrint.Stop();
    m_Connector->EndConnectionAttempt();
}