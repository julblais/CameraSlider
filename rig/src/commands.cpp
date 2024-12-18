#include "commands.h"

const char* SpeedMaxCommand::GetTitle()
{
    return "Vitesse max.";
}

const char* SpeedCurveCommand::GetTitle()
{
    return "Courbe vitesse";
}

const char* SpeedMaxCommand::GetDesc()
{
    if (m_Speed == SpeedMaxCommand::Speed::SLOW)
        return "Pas vite";
    else if (m_Speed == SpeedMaxCommand::Speed::SMED)
        return "Moyen vite";
    else if (m_Speed == SpeedMaxCommand::Speed::SHIGH)
        return "Fuckin vite";
    else return nullptr;
}

 void SpeedMaxCommand::Invoke(Utils::MenuCommandButton command)
{
    if (m_Speed == SpeedMaxCommand::Speed::SLOW)
        m_Speed = SpeedMaxCommand::Speed::SMED;
    else if (m_Speed == SpeedMaxCommand::Speed::SMED)
        m_Speed = SpeedMaxCommand::Speed::SHIGH;
    else if (m_Speed == SpeedMaxCommand::Speed::SHIGH)
        m_Speed = SpeedMaxCommand::Speed::SLOW;
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