#include "commands.h"
#include "src/app/settings.h"

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
    if (speed == Settings::SpeedSlow)
        Settings::GetInstance().SetSpeed(Settings::SpeedMedium);
    else if (speed == Settings::SpeedMedium)
        Settings::GetInstance().SetSpeed(Settings::SpeedHigh);
    else if (speed == Settings::SpeedHigh)
        Settings::GetInstance().SetSpeed(Settings::SpeedSlow);
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
    if (curve == Settings::CurveLinear)
        Settings::GetInstance().SetCurve(Settings::CurveQuadratic);
    else if (curve == Settings::CurveQuadratic)
        Settings::GetInstance().SetCurve(Settings::CurveExponential);
    else if (curve == Settings::CurveExponential)
        Settings::GetInstance().SetCurve(Settings::CurveLinear);
}