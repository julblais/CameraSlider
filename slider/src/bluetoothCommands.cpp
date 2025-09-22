#include "bluetoothCommands.h"

#if !IS_SIMULATOR

#include "bluetoothComponent.h"

using namespace Bt;
using namespace Core;

GamepadNameCommand::GamepadNameCommand(BluetoothComponent* bluetooth)
    : m_Bluetooth(bluetooth) {}

void GamepadNameCommand::Print(Display* display) const
{
    const auto name = m_Bluetooth->GetGamepad().GetDescription();
    PrintTitle(display, "Manette");
    if (name.get() == nullptr)
        PrintDescription(display, "Aucune", false);
    else
        PrintDescription(display, name.get(), false);
}

void GamepadNameCommand::Invoke(const MenuCommandButton command)
{
    if (m_Bluetooth != nullptr)
    {
        const auto gamepad = m_Bluetooth.GetGamepad();
        if (!gamepad.IsConnected())
        {
            m_Bluetooth->EnablePairing();
        }
    }
}

#endif
