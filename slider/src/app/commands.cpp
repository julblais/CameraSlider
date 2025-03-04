#include "commands.h"
#include "settings.h"
#include "src/core/utils/mathUtils.h"
#include "src/network/wifiModule.h"

using namespace Core;
using namespace Slider;

void BrainAddressCommand::Print(Display* display) const
{
    PrintTitle(display, "Adr. mac");
    display->PrintLine(1, " ", Net::WifiModule::GetInstance().GetMacAddress());
}

void ControllerAddressCommand::Print(Display* display) const
{
    PrintTitle(display, "Adr. manette");
    if (Settings::GetInstance().HasPeerAddress())
        display->PrintLine(1, " ", Settings::GetInstance().GetPeerAddress());
    else
        display->PrintLine(1, " ", "Aucune manette");
}

void ConnectionCommand::Print(Display* display) const
{
    PrintTitle(display, "Connect manett");
    const auto rightArrow = display->GetSymbol(Symbol::RightArrow);
    display->PrintLine(1, " ", rightArrow, "Oui  Non");
}

void ConnectionCommand::Invoke(MenuCommandButton command)
{
    if (m_State == State::NotConnected)
    {
        if (command == MenuCommandButton::SELECT)
        {
            m_State == State::Connecting;
            m_Connector = std::unique_ptr<BrainConnector>(new BrainConnector());
            m_Connector.get()->Setup();
        }
    }
}

void ConnectionCommand::OnUpdate()
{
    if (m_Connector != nullptr)
        m_Connector->Update();
}

void ConnectionCommand::OnShow()
{

}

void ConnectionCommand::OnHide()
{
    if (m_Connector)
        m_Connector.reset();
}
