#include "wifiModule.h"
#include "src/debug.h"

using namespace Net;

WifiModule& WifiModule::GetInstance()
{
    static WifiModule instance;
    return instance;
}

#ifndef IS_SIMULATOR

#include "src/network/esp.h"

void WifiModule::Setup()
{
    Esp::Init();

    Esp::RegisterReceiveCb([this](const uint8_t* data, size_t length) {
        m_MessageHandler.Invoke(data, length);
    });
}

void WifiModule::Update()
{
    m_MessageHandler.ProcessMessages();
}

MacAddress WifiModule::GetMacAddress()
{
    return Esp::GetMacAddress();
}

bool WifiModule::AddPeer(const MacAddress& address)
{
    return Esp::AddPeer(address);
}

bool WifiModule::RemovePeer(const MacAddress& address)
{
    return Esp::RemovePeer(address);
}

void Net::WifiModule::RemoveReceiveCallback(const MessageCallbackHandle& handle)
{
    m_MessageHandler.RemoveCallback(handle);
}

void WifiModule::RegisterSendCallback(Core::function<void(const MacAddress&, bool)> callback)
{
    Esp::RegisterSendCallback(callback);
}

bool WifiModule::SendImpl(const uint8_t* address, const uint8_t* data, size_t len)
{
    return Esp::Send(address, data, len);
}

#else

Core::function<void(const MacAddress&, bool)> s_SendCallback {};

void WifiModule::Setup()
{
    LogInfo("Bypassing network initialization.");
}

void WifiModule::Update()
{
    m_MessageHandler.ProcessMessages();
}

MacAddress WifiModule::GetMacAddress()
{
    return MacAddress { {0x01, 0x02, 0x03, 0x04, 0x05, 0x06} };
}

bool WifiModule::AddPeer(const MacAddress& address)
{
    return true;
}

bool WifiModule::RemovePeer(const MacAddress& address)
{
    return true;
}

void WifiModule::RegisterSendCallback(Core::function<void(const MacAddress&, bool)> callback)
{
    s_SendCallback = callback;
}

void Net::WifiModule::RemoveReceiveCallback(const MessageCallbackHandle& handle)
{
    m_MessageHandler.RemoveCallback(handle);
}

bool WifiModule::SendImpl(const uint8_t* address, const uint8_t* data, size_t len)
{
    if (s_SendCallback)
        s_SendCallback(MacAddress(address), true);
    m_OtherMessageHandler.Invoke(data, len);
    m_OtherMessageHandler.ProcessMessages();
    return true;
}

#endif