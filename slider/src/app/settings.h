#ifndef SETTINGS_H
#define SETTINGS_H

namespace Slider
{
    struct Settings
    {
        enum class Speed { S_SLOW = 0, S_MED = 1, S_HIGH = 2 };
        enum class Curve { C_LINEAR = 0, C_QUAD = 1, C_EXP = 2 };
        
        static constexpr auto SpeedSlow = Speed::S_SLOW;
        static constexpr auto SpeedMedium = Speed::S_MED;
        static constexpr auto SpeedHigh = Speed::S_HIGH;
        static constexpr auto CurveLinear = Curve::C_LINEAR;
        static constexpr auto CurveQuadratic = Curve::C_QUAD;
        static constexpr auto CurveExponential = Curve::C_EXP;

        inline Speed GetSpeed() const { return m_Speed; }
        inline Curve GetCurve() const { return m_Curve; }

        inline void SetSpeed(Speed value) { m_Speed = value; }
        inline void SetCurve(Curve value) { m_Curve = value; }
        

        static Settings Default()
        {
            auto s = Settings();
            s.SetSpeed(SpeedMedium);
            s.SetCurve(CurveLinear);
            return s;
        }

        private:
            Speed m_Speed;
            Curve m_Curve;
    };

    static Settings AppSettings = Settings::Default();
}

#endif