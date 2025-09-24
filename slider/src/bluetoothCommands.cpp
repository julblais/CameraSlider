#include "bluetoothCommands.h"

#if !IS_SIMULATOR

#include "bluetoothComponent.h"
#include "bluetoothGamepad.h"

using namespace Bt;
using namespace Core;

GamepadNameCommand::GamepadNameCommand(BluetoothComponent* bluetooth)
    : m_Bluetooth(bluetooth) {}

void GamepadNameCommand::Print(Display* display) const
{
    const auto name = m_Bluetooth->GetGamepad()->GetDescription();
    PrintTitle(display, "Manette");
    if (name.get() == nullptr)
        PrintDescription(display, DescriptionType::None, "Aucune");
    else
        PrintDescription(display, DescriptionType::None, name.get());
}

GamepadConnectionCommand::GamepadConnectionCommand(BluetoothComponent* bluetooth)
    : m_Bluetooth(bluetooth),
      m_IsPairing(false),
      m_Progress(AnimatedPrintable::CreateProgressDots()),
      m_ShowConnectionResult(false),
      m_ConnectionResultTimer(Timer::Create("SerialDisplay", [this]() { m_ShowConnectionResult = false; })) {}

void GamepadConnectionCommand::Print(Display* display) const
{
    if (m_ShowConnectionResult)
    {
        const auto gamepad = m_Bluetooth->GetGamepad();
        display->PrintLine(0, "Manette trouvee");
        display->PrintLine(1, " ", gamepad->GetDescription().get());
        return;
    }

    PrintTitle(display, "Bluetooth");
    const auto gamepad = m_Bluetooth->GetGamepad();
    if (gamepad->IsConnected())
        PrintDescription(display, DescriptionType::Action, "Reset?");
    else if (m_IsPairing == false)
        PrintDescription(display, DescriptionType::Action, "Connexion?");
    else
        PrintDescription(display, DescriptionType::Action, "Recherche", m_Progress);
}

void GamepadConnectionCommand::Invoke(const Button command)
{
    if (command != Button::Select)
        return;

    if (m_Bluetooth == nullptr)
    {
        LogError("GamepadConnectionCommand: Cannot reset, Bluetooth component is null");
        return;
    }

    if (m_IsPairing)
    {
        LogInfo("GamepadConnectionCommand: Cannot reset, already pairing");
        return;
    }

    const auto gamepad = m_Bluetooth->GetGamepad();
    if (!gamepad->IsConnected())
    {
        m_Bluetooth->EnablePairing();
        m_IsPairing = true;
    }
    else
    {
        m_Bluetooth->DisconnectGamepad();
        m_Bluetooth->Reset();
        m_IsPairing = false;
    }
}

void GamepadConnectionCommand::OnUpdate()
{
    if (m_IsPairing) //check gamepad connection
    {
        const auto gamepad = m_Bluetooth->GetGamepad();
        if (gamepad->IsConnected())
        {
            LogInfo("GamepadConnectionCommand: Gamepad connected");
            m_IsPairing = false;
            m_ShowConnectionResult = true;
            m_ConnectionResultTimer.Start(3000, false);
            m_Bluetooth->DisablePairing();
        }
    }
}

void GamepadConnectionCommand::OnHide()
{
    if (m_IsPairing)
    {
        m_IsPairing = false;
        m_ShowConnectionResult = false;
        m_Bluetooth->DisablePairing();
    }
}

#endif
