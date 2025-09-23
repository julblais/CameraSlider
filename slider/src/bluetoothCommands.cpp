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
        PrintDescription(display, "Aucune", false);
    else
        PrintDescription(display, name.get(), false);
}

GamepadConnectionCommand::GamepadConnectionCommand(BluetoothComponent* bluetooth)
    : m_Bluetooth(bluetooth) {}

void GamepadConnectionCommand::Print(Display* display) const
{
    PrintTitle(display, "Bluetooth");
    const auto gamepad = m_Bluetooth->GetGamepad();
    if (gamepad->IsConnected())
        PrintDescription(display, "Deconnecter", false);
    else
        PrintDescription(display, "Connecter", false);
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
        }
    }
}

#endif
