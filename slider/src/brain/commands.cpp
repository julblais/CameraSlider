#include "commands.h"
#include "settings.h"
#include "src/core/utils/mathUtils.h"

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

void MaxSpeedCommand::Print(Output::Display* display) const
{
    PrintTitle(display, "Vitesse max.");
    auto desc = GetDesc(Settings::GetInstance().GetSpeed());
    PrintDescription(display, desc);
}

void SpeedCurveCommand::Print(Output::Display* display) const
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

BrainMacAddress::BrainMacAddress(Net::WifiModule* wifiModule)
{}

const char* BrainMacAddress::GetTitle()
{
    return "Mac address";
}

const char* BrainMacAddress::GetDesc()
{
    return m_Wifi->GetMacAddress().c_str();
}

void BrainMacAddress::Invoke(MenuCommandButton command) {}

const char* ControllerMacAddress::GetTitle()
{
    return "Ctrl. address";
}

const char* ControllerMacAddress::GetDesc()
{
    return nullptr;
}

void ControllerMacAddress::Invoke(MenuCommandButton command) {}
