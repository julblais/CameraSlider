#include "commands.h"

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
    if (m_Speed == MaxSpeedCommand::Speed::SLOW)
        return "Pas vite";
    else if (m_Speed == MaxSpeedCommand::Speed::SMED)
        return "Moyen vite";
    else if (m_Speed == MaxSpeedCommand::Speed::SHIGH)
        return "Fuckin vite";
    else return nullptr;
}

 void MaxSpeedCommand::Invoke(Utils::MenuCommandButton command)
{
    if (m_Speed == MaxSpeedCommand::Speed::SLOW)
        m_Speed = MaxSpeedCommand::Speed::SMED;
    else if (m_Speed == MaxSpeedCommand::Speed::SMED)
        m_Speed = MaxSpeedCommand::Speed::SHIGH;
    else if (m_Speed == MaxSpeedCommand::Speed::SHIGH)
        m_Speed = MaxSpeedCommand::Speed::SLOW;
}

const char* SpeedCurveCommand::GetDesc()
{
    if (m_Curve == SpeedCurveCommand::Curve::CLINEAR)
        return "Lineaire";
    else if (m_Curve == SpeedCurveCommand::Curve::CQUAD)
        return "Quadratique";
    else if (m_Curve == SpeedCurveCommand::Curve::CEXP)
        return "Exponentielle";
    else return nullptr;
}

void SpeedCurveCommand::Invoke(Utils::MenuCommandButton command)
{
    if (m_Curve == SpeedCurveCommand::Curve::CLINEAR)
        m_Curve = SpeedCurveCommand::Curve::CQUAD;
    else if (m_Curve == SpeedCurveCommand::Curve::CQUAD)
        m_Curve = SpeedCurveCommand::Curve::CEXP;
    else if (m_Curve == SpeedCurveCommand::Curve::CEXP)
        m_Curve = SpeedCurveCommand::Curve::CLINEAR;
}