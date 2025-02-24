#ifndef SETTINGS_H
#define SETTINGS_H

#include <Preferences.h>

namespace Slider
{
    struct Settings
    {
        enum class Speed : char { S_SLOW, S_MED, S_HIGH, COUNT };
        enum class Curve : char { C_LINEAR, C_QUAD, C_EXP, COUNT };

        static constexpr auto SpeedSlow = Speed::S_SLOW;
        static constexpr auto SpeedMedium = Speed::S_MED;
        static constexpr auto SpeedHigh = Speed::S_HIGH;

        static constexpr auto CurveLinear = Curve::C_LINEAR;
        static constexpr auto CurveQuadratic = Curve::C_QUAD;
        static constexpr auto CurveExponential = Curve::C_EXP;

        Speed GetSpeed() const;
        Curve GetCurve() const;

        void SetSpeed(Speed value);
        void SetCurve(Curve value);

        static Settings& GetInstance();

    private:
        Settings();
        Settings(const Settings&) = delete;

        mutable Preferences m_Prefs;
    };
}

#endif