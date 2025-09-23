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
      m_ConnectionMsg(AnimatedPrintable(500, {"Connexion", "Connexion.", "Connexion..", "Connexion..."})) {}

void GamepadConnectionCommand::Print(Display* display) const
{
    PrintTitle(display, "Bluetooth");
    const auto gamepad = m_Bluetooth->GetGamepad();
    if (gamepad->IsConnected())
        PrintDescription(display, DescriptionType::Action, "Reset");
    else if (m_IsPairing == false)
        PrintDescription(display, DescriptionType::Action, "Connecter");
    else
        PrintDescription(display, DescriptionType::Action, m_ConnectionMsg);
}

void GamepadConnectionCommand::Invoke(const Button command)
{
    if (m_Bluetooth != nullptr)
    {
        const auto gamepad = m_Bluetooth->GetGamepad();
        if (!gamepad->IsConnected())
        {
            m_Bluetooth->EnablePairing();
            //needs more advanced code (state machine to manage pairing)
        }
        else
        {
            m_Bluetooth->DisconnectGamepad();
            m_Bluetooth->Reset();
        }
    }
}

void GamepadConnectionCommand::OnUpdate() {}

void GamepadConnectionCommand::OnHide() {}

#endif
