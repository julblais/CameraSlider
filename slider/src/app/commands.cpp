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
    switch (AppSettings.GetSpeed())
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
    const auto speed = AppSettings.GetSpeed();
    if (speed == Settings::SpeedSlow)
        AppSettings.SetSpeed(Settings::SpeedMedium);
    else if (speed == Settings::SpeedMedium)
        AppSettings.SetSpeed(Settings::SpeedHigh);
    else if (speed == Settings::SpeedHigh)
        AppSettings.SetSpeed(Settings::SpeedSlow);
}

const char* Slider::SpeedCurveCommand::GetDesc()
{
    switch (AppSettings.GetCurve())
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
    const auto curve = AppSettings.GetCurve();
    if (curve == Settings::CurveLinear)
        AppSettings.SetCurve(Settings::CurveQuadratic);
    else if (curve == Settings::CurveQuadratic)
        AppSettings.SetCurve(Settings::CurveExponential);
    else if (curve == Settings::CurveExponential)
        AppSettings.SetCurve(Settings::CurveLinear);
}