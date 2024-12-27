#include "settings.h"

#include <EEPROM.h>

constexpr auto SPEED_ADDR = 0;
constexpr auto CURVE_ADDR = 1;

Slider::Settings& Slider::Settings::GetInstance()
{
    static Settings instance;
    return instance;
}

void Slider::Settings::Save()
{
    EEPROM.write(SPEED_ADDR, static_cast<int>(m_Speed));
    EEPROM.write(CURVE_ADDR, static_cast<int>(m_Curve));
    EEPROM.commit();
}

void Slider::Settings::Restore()
{
    m_Speed = static_cast<Speed>(EEPROM.read(SPEED_ADDR));
    m_Curve = static_cast<Curve>(EEPROM.read(CURVE_ADDR));
}