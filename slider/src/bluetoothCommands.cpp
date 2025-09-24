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
    display->PrintLine(0, TitlePrefix(), "Manette");
    if (name.get() == nullptr)
        display->PrintLine(1, DescriptionPrefix(DescriptionType::None), "Aucune");
    else
        display->PrintLine(1, DescriptionPrefix(DescriptionType::None), name.get());
}

GamepadConnectionCommand::GamepadConnectionCommand(BluetoothComponent* bluetooth)
    : m_Bluetooth(bluetooth),
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

    display->PrintLine(0, TitlePrefix(), "Bluetooth");
    const auto gamepad = m_Bluetooth->GetGamepad();
    if (gamepad->IsConnected())
        display->PrintLine(1, DescriptionPrefix(DescriptionType::Action), "Reset?");
    else if (m_Bluetooth->IsPairing())
        display->PrintLine(1, DescriptionPrefix(DescriptionType::Action), "Recherche", m_Progress);
    else
        display->PrintLine(1, DescriptionPrefix(DescriptionType::Action), "Connexion?");
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

    if (m_Bluetooth->IsPairing())
    {
        LogInfo("GamepadConnectionCommand: Cannot reset, already pairing");
        return;
    }

    const auto gamepad = m_Bluetooth->GetGamepad();
    if (!gamepad->IsConnected())
    {
        m_Bluetooth->EnablePairing();
    }
    else
    {
        m_Bluetooth->DisconnectGamepad();
        m_Bluetooth->Reset();
    }
}

void GamepadConnectionCommand::OnUpdate()
{
    if (m_Bluetooth->IsPairing()) //check gamepad connection
    {
        const auto gamepad = m_Bluetooth->GetGamepad();
        if (gamepad->IsConnected())
        {
            LogInfo("GamepadConnectionCommand: Gamepad connected");
            m_ShowConnectionResult = true;
            m_ConnectionResultTimer.Start(3000, false);
            m_Bluetooth->DisablePairing();
        }
    }
}

void GamepadConnectionCommand::OnHide()
{
    if (m_Bluetooth->IsPairing())
    {
        m_ShowConnectionResult = false;
        m_Bluetooth->DisablePairing();
    }
}

#endif
