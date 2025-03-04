#include "curveCommand.h"
#include "src/app/settings.h"
#include "src/core/utils/mathUtils.h"

using namespace Core;
using namespace Slider;

const char* GetDesc(Settings::Curve curve)
{
    switch (curve)
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

void SpeedCurveCommand::Invoke(MenuCommandButton command)
{
    const auto curve = Settings::GetInstance().GetCurve();
    if (command == MenuCommand::ButtonLeft)
        Settings::GetInstance().SetCurve(GetPreviousEnumValue(curve));
    else if (command == MenuCommand::ButtonRight)
        Settings::GetInstance().SetCurve(GetNextEnumValue(curve));
}

void SpeedCurveCommand::Print(Display* display) const
{
    PrintTitle(display, "Courbe vitesse");
    auto desc = GetDesc(Settings::GetInstance().GetCurve());
    PrintDescription(display, desc);
}