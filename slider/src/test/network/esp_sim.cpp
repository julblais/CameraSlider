#ifdef IS_SIMULATOR

#include "esp_sim.h"
#include "src/debug.h"
#include "address.h"

#include <functional>

using namespace Network;

MessageHandler Esp::s_ReceiveHandler;
MessageHandler Esp::s_SendHandler;

bool Esp::Init()
{
    LogInfo("Bypassing network initialization.");
    return true;
}

MacAddress Esp::GetMacAddress(){ return INVALID_ADDRESS; }
bool Esp::AddPeer(const MacAddress& address) { return true; }
bool Esp::RemovePeer(const MacAddress& address) { return true; }
void Esp::RegisterSendCallback(const Esp::SendCallback& callback) { }

bool Network::Esp::Send(const MacAddress &address, const uint8_t* data, size_t len)
{
    return Send(data, len);
}

bool Network::Esp::Send(const uint8_t *data, size_t len)
{
    s_SendHandler.Invoke(data, len);
    return true;
}

#endif