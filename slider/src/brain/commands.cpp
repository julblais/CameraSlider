#include "commands.h"
#include "settings.h"
#include "src/core/utils/mathUtils.h"
#include "src/network/wifiModule.h"

using namespace Core;
using namespace Slider;

const char* GetDesc(Settings::Curve curve)
{
    switch (curve)
    {
        case Settings::CurveLinear:
            return "Lineaire";
        case Settings::CurveQuadratic:
            return "Quadratique";
        case Settings::CurveExponential:
            return "Exponentielle";
        default:
            return nullptr;
    };
}

const char* GetDesc(Settings::Speed speed)
{
    switch (speed)
    {
        case Settings::SpeedSlow:
            return "Pas vite";
        case Settings::SpeedMedium:
            return "Moyen vite";
        case Settings::SpeedHigh:
            return "Fuckin vite";
        default:
            return nullptr;
    };
}

void MaxSpeedCommand::Print(Display* display) const
{
    PrintTitle(display, "Vitesse max.");
    auto desc = GetDesc(Settings::GetInstance().GetSpeed());
    PrintDescription(display, desc);
}

void SpeedCurveCommand::Print(Display* display) const
{
    PrintTitle(display, "Courbe vitesse");
    auto desc = GetDesc(Settings::GetInstance().GetCurve());
    PrintDescription(display, desc);
}

void MaxSpeedCommand::Invoke(MenuCommandButton command)
{
    const auto speed = Settings::GetInstance().GetSpeed();
    if (command == MenuCommand::ButtonLeft)
        Settings::GetInstance().SetSpeed(GetPreviousEnumValue(speed));
    else if (command == MenuCommand::ButtonRight)
        Settings::GetInstance().SetSpeed(GetNextEnumValue(speed));
}

void SpeedCurveCommand::Invoke(MenuCommandButton command)
{
    const auto curve = Settings::GetInstance().GetCurve();
    if (command == MenuCommand::ButtonLeft)
        Settings::GetInstance().SetCurve(GetPreviousEnumValue(curve));
    else if (command == MenuCommand::ButtonRight)
        Settings::GetInstance().SetCurve(GetNextEnumValue(curve));
}

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
