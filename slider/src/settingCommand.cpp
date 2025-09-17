#include "settingCommand.h"
#include "settings.h"
#include "core/utils/enumUtils.h"
#include "bluetoothComponent.h"

using namespace Core;
using namespace Slider;

const char* GetDesc(const Settings::Speed speed)
{
    switch (speed)
    {
        case Settings::SpeedSlow:
            return "Pas vite";
        case Settings::SpeedMedium:
            return "Moyen vite";
        case Settings::SpeedHigh:
            return "Fuckin vite";
        default:
            return nullptr;
    };
}

const char* GetDesc(const Settings::Curve curve)
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

void MaxSpeedCommand::Print(Display* display) const
{
    PrintTitle(display, "Vitesse max.");
    const auto desc = GetDesc(Settings::GetInstance().GetSpeed());
    PrintDescription(display, desc);
}

void MaxSpeedCommand::Invoke(const MenuCommandButton command)
{
    const auto speed = Settings::GetInstance().GetSpeed();
    if (command == ButtonLeft)
        Settings::GetInstance().SetSpeed(GetPreviousEnumValue(speed));
    else if (command == ButtonRight)
        Settings::GetInstance().SetSpeed(GetNextEnumValue(speed));
}

void SpeedCurveCommand::Invoke(const MenuCommandButton command)
{
    const auto curve = Settings::GetInstance().GetCurve();
    if (command == ButtonLeft)
        Settings::GetInstance().SetCurve(GetPreviousEnumValue(curve));
    else if (command == ButtonRight)
        Settings::GetInstance().SetCurve(GetNextEnumValue(curve));
}

void SpeedCurveCommand::Print(Display* display) const
{
    PrintTitle(display, "Courbe vitesse");
    const auto desc = GetDesc(Settings::GetInstance().GetCurve());
    PrintDescription(display, desc);
}

GamepadPairCommand::GamepadPairCommand(Bt::BluetoothComponent* bluetooth)
    : m_Bluetooth(bluetooth) {}

void GamepadPairCommand::Print(Display* display) const
{
    PrintTitle(display, "Gamepad pair");
    PrintDescription(display, "None");
}

void GamepadPairCommand::Invoke(const MenuCommandButton command)
{
    if (m_Bluetooth != nullptr)
    {
        const auto gamepad = m_Bluetooth->GetGamepad();
        if (gamepad == nullptr || !gamepad->IsConnected())
        {
            m_Bluetooth->EnablePairing();
        }
    }
}
