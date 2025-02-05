#ifdef IS_SIMULATOR

#include "esp_sim.h"
#include "src/debug.h"
#include "address.h"

#include <functional>

using namespace Network;

MessageHandler Esp::s_Handler;
MessageHandler Esp::s_SimulateHandler;
Esp::SendCallback s_SendCallback;

bool Esp::Init()
{
    LogInfo("Bypassing network initialization.");
    return true;
}

MacAddress Esp::GetMacAddress(){ return MacAddress{{0x01, 0x02, 0x03, 0x04, 0x05, 0x06}}; }
bool Esp::AddPeer(const MacAddress& address) { return true; }
bool Esp::RemovePeer(const MacAddress& address) { return true; }
void Esp::RegisterSendCallback(const Esp::SendCallback& callback) { s_SendCallback = callback; }

bool Network::Esp::Send(const uint8_t *data, size_t len)
{
    if (s_SendCallback)
        s_SendCallback(INVALID_ADDRESS, true);
    s_SimulateHandler.Invoke(data, len);
    return true;
}

#endif