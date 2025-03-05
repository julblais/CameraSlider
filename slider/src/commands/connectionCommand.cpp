#include "connectionCommand.h"
#include "settings.h"
#include "src/network/wifiModule.h"

using namespace Core;
using namespace Slider;

ConnectionCommand::ConnectionCommand(Slider::BrainConnector* connector)
    :m_Connector(connector)
{}

void ConnectionCommand::Print(Display* display) const
{
    PrintTitle(display, "Connect manett");
    const auto rightArrow = display->GetSymbol(Symbol::RightArrow);
    display->PrintLine(1, " ", rightArrow, "Oui  Non");
}

void ConnectionCommand::Invoke(MenuCommandButton command)
{
    if (m_Connector->GetState() != BrainConnector::ConnectionState::CONNECTED)
    {
        if (command == MenuCommandButton::SELECT)
            m_Connector->BeginConnectionAttempt();
    }
}

void ConnectionCommand::OnHide()
{
    m_Connector->EndConnectionAttempt();
}