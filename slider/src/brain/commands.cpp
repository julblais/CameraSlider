#include "commands.h"
#include "settings.h"
#include "src/core/utils/mathUtils.h"

using namespace Core;
using namespace Slider;

const char* MaxSpeedCommand::GetTitle()
{
    return "Vitesse max.";
}

const char* SpeedCurveCommand::GetTitle()
{
    return "Courbe vitesse";
}

const char* MaxSpeedCommand::GetDesc()
{
    switch (Settings::GetInstance().GetSpeed())
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

void MaxSpeedCommand::Invoke(MenuCommandButton command)
{
    const auto speed = Settings::GetInstance().GetSpeed();
    if (command == MenuCommand::ButtonLeft)
        Settings::GetInstance().SetSpeed(GetPreviousEnumValue(speed));
    else if (command == MenuCommand::ButtonRight)
        Settings::GetInstance().SetSpeed(GetNextEnumValue(speed));
}

const char* SpeedCurveCommand::GetDesc()
{
    switch (Settings::GetInstance().GetCurve())
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
