#ifndef SETTINGS_H
#define SETTINGS_H

#include <Preferences.h>

namespace Slider
{
    struct Settings
    {
        enum class Speed : char { S_MED = 0, S_SLOW = 1, S_HIGH = 2 };
        enum class Curve : char { C_LINEAR = 0, C_QUAD = 1, C_EXP = 2 };
        
        static constexpr auto SpeedSlow = Speed::S_SLOW;
        static constexpr auto SpeedMedium = Speed::S_MED;
        static constexpr auto SpeedHigh = Speed::S_HIGH;
        static constexpr auto CurveLinear = Curve::C_LINEAR;
        static constexpr auto CurveQuadratic = Curve::C_QUAD;
        static constexpr auto CurveExponential = Curve::C_EXP;

        Settings();

        Speed GetSpeed() const;
        Curve GetCurve() const;

        void SetSpeed(Speed value);
        void SetCurve(Curve value);
        
        static Settings& GetInstance();

        private:
            mutable Preferences m_Prefs;
    };
}

#endif