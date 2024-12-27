#include "commands.h"
#include "src/app/settings.h"
#include "src/utils/mathUtils.h"

const char* Slider::MaxSpeedCommand::GetTitle()
{
    return "Vitesse max.";
}

const char* Slider::SpeedCurveCommand::GetTitle()
{
    return "Courbe vitesse";
}

const char* Slider::MaxSpeedCommand::GetDesc()
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

void Slider::MaxSpeedCommand::Invoke(Utils::MenuCommandButton command)
{
    const auto speed = Settings::GetInstance().GetSpeed();
    if (command == MenuCommand::ButtonLeft)
        Settings::GetInstance().SetSpeed(GetPreviousEnumValue(speed));
    else if (command == MenuCommand::ButtonRight)
        Settings::GetInstance().SetSpeed(GetNextEnumValue(speed));
}

const char* Slider::SpeedCurveCommand::GetDesc()
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

void Slider::SpeedCurveCommand::Invoke(Utils::MenuCommandButton command)
{
    const auto curve = Settings::GetInstance().GetCurve();
    if (command == MenuCommand::ButtonLeft)
        Settings::GetInstance().SetCurve(GetPreviousEnumValue(curve));
    else if (command == MenuCommand::ButtonRight)
        Settings::GetInstance().SetCurve(GetNextEnumValue(curve));
}