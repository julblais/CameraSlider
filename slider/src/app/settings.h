#ifndef SETTINGS_H
#define SETTINGS_H

namespace Slider
{
    struct Settings
    {
        enum class Speed { S_MED = 0, S_SLOW = 1, S_HIGH = 2 };
        enum class Curve { C_LINEAR = 0, C_QUAD = 1, C_EXP = 2 };
        
        static constexpr auto SpeedSlow = Speed::S_SLOW;
        static constexpr auto SpeedMedium = Speed::S_MED;
        static constexpr auto SpeedHigh = Speed::S_HIGH;
        static constexpr auto CurveLinear = Curve::C_LINEAR;
        static constexpr auto CurveQuadratic = Curve::C_QUAD;
        static constexpr auto CurveExponential = Curve::C_EXP;

        Speed GetSpeed() const { return m_Speed; }
        Curve GetCurve() const { return m_Curve; }

        void SetSpeed(Speed value) { m_Speed = value; }
        void SetCurve(Curve value) { m_Curve = value; }

        void Save();
        void Restore();
        
        static Settings& GetInstance();

        private:
            Speed m_Speed;
            Curve m_Curve;
    };
}

#endif