#include "settings.h"

#include <Preferences.h>

constexpr auto SPEED_KEY = "speed";
constexpr auto CURVE_KEY = "curve";
constexpr auto PEER_KEY = "peer";

Slider::Settings::Settings() :
    m_Prefs()
{
    m_Prefs.begin("settings");
}

Slider::Settings::Speed Slider::Settings::GetSpeed() const
{
    static constexpr auto s_default = static_cast<char>(SpeedMedium);
    return static_cast<Speed>(m_Prefs.getChar(SPEED_KEY, s_default));
}

Slider::Settings::Curve Slider::Settings::GetCurve() const
{
    static constexpr auto s_default = static_cast<char>(CurveLinear);
    return static_cast<Curve>(m_Prefs.getChar(CURVE_KEY, s_default));
}

void Slider::Settings::SetSpeed(Speed value)
{
    m_Prefs.putChar(SPEED_KEY, static_cast<char>(value));
}

void Slider::Settings::SetCurve(Curve value)
{
    m_Prefs.putChar(CURVE_KEY, static_cast<char>(value));
}

bool Slider::Settings::HasPeerAddress() const
{
    return m_Prefs.isKey(PEER_KEY);
}

Core::MacAddress Slider::Settings::GetPeerAddress() const
{
    uint8_t buffer[6];
    m_Prefs.getBytes(PEER_KEY, &buffer, 6);
    return Core::MacAddress(buffer);
}

void Slider::Settings::SetPeerAddress(const Core::MacAddress& address) const
{
    m_Prefs.putBytes(PEER_KEY, address.Data(), address.Size());
}

Slider::Settings& Slider::Settings::GetInstance()
{
    static Settings instance;
    return instance;
}