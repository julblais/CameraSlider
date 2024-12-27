#include "settings.h"

#include <Preferences.h>

constexpr auto SPEED_KEY = "speed";
constexpr auto CURVE_KEY = "curve";

Slider::Settings::Settings():
    m_Prefs()
{
    m_Prefs.begin("settings");
}

Slider::Settings::Speed Slider::Settings::GetSpeed() const 
{
    return static_cast<Speed>(m_Prefs.getChar(SPEED_KEY, 0));
}

Slider::Settings::Curve Slider::Settings::GetCurve() const
{
    return static_cast<Curve>(m_Prefs.getChar(CURVE_KEY, 0));
}

void Slider::Settings::SetSpeed(Speed value)
{
    m_Prefs.putChar(SPEED_KEY, static_cast<char>(value));
}

void Slider::Settings::SetCurve(Curve value)
{
    m_Prefs.putChar(CURVE_KEY, static_cast<char>(value));
}

Slider::Settings& Slider::Settings::GetInstance()
{
    static Settings instance;
    return instance;
}