#ifndef IS_SIMULATOR

#include "esp.h"
#include "src/debug.h"
#include "src/core/network/address.h"

#include <functional>

#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

using namespace Net;
using namespace Core;

#define SEMAPHORE_WAIT_MS 1000
#define TOO_LONG_WARNING_MS 10
#define CHECK_ERROR_RETURN(message, error) if (HasError(message, error)) { return false; }

Esp::SendCallback s_SendCallback {};
Esp::ReceiveCallback s_ReceiveCallback {};
SemaphoreHandle_t semaphore {};

void OnReceive(const uint8_t* mac_addr, const uint8_t* data, size_t length)
{
    //LogDebug("Received message from: ", MacAddress(mac_addr));
    if (s_ReceiveCallback)
        s_ReceiveCallback(data, length);
}

void OnSend(const uint8_t* mac_addr, esp_now_send_status_t status)
{
    MacAddress address(mac_addr);
    if (s_SendCallback)
        s_SendCallback(address, status == ESP_NOW_SEND_SUCCESS);
    if (status != ESP_NOW_SEND_SUCCESS)
        LogError("Message was not sent");
    xSemaphoreGive(semaphore);
}

bool HasError(const char* error, esp_err_t result)
{
    if (result != ESP_OK)
    {
        LogError(error, esp_err_to_name(result));
        return true;
    }
    return false;
}

bool Esp::Init()
{
    semaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(semaphore);

    LogInfo("Init network...");

    LogDebug("Init wifi...");
    WiFi.useStaticBuffers(true);
    WiFi.mode(WIFI_STA);
    LogInfo("Wifi channel: ", WiFi.channel());

    LogDebug("Init espnow...");
    CHECK_ERROR_RETURN("Error initializing ESP-NOW: ", esp_now_init());
    CHECK_ERROR_RETURN("Failed to register receive callback: ", esp_now_register_recv_cb(esp_now_recv_cb_t(OnReceive)));
    CHECK_ERROR_RETURN("Failed to register send callback: ", esp_now_register_send_cb(esp_now_send_cb_t(OnSend)));

    return true;
}

MacAddress Esp::GetMacAddress()
{
    std::array<uint8_t, 6> address;
    if (!HasError("Failed to read mac address: ", esp_wifi_get_mac(WIFI_IF_STA, address.data())))
        return MacAddress(address);
    else
        return INVALID_ADDRESS;
}

bool Esp::AddPeer(const MacAddress& address)
{
    esp_now_peer_info_t peerInfo;
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    peerInfo.ifidx = WIFI_IF_STA;
    address.CopyTo(peerInfo.peer_addr);

    CHECK_ERROR_RETURN("Failed to add peer ", esp_now_add_peer(&peerInfo));

    LogDebug("Added peer: ", address);
    return true;
}

bool Esp::RemovePeer(const MacAddress& address)
{
    CHECK_ERROR_RETURN("Failed to remove peer ", esp_now_del_peer(address.Data()));

    LogDebug("Removed peer: ", address);
    return true;
}

void Esp::RegisterReceiveCb(const ReceiveCallback& callback)
{
    s_ReceiveCallback = callback;
}

void Esp::RegisterSendCallback(const Esp::SendCallback& callback)
{
    s_SendCallback = callback;
}

bool Esp::Send(const uint8_t* address, const uint8_t* data, size_t len)
{
    auto time = esp_timer_get_time();
    if (xSemaphoreTake(semaphore, pdMS_TO_TICKS(SEMAPHORE_WAIT_MS)))
    {
        auto delayMs = (esp_timer_get_time() - time) / 1000;
        if (delayMs > TOO_LONG_WARNING_MS)
            LogWarning("Message send delay: ", delayMs, "ms");
        CHECK_ERROR_RETURN("Error sending message: ", esp_now_send(address, data, len));
        return true;
    }
    LogError("Message was not sent: took to long to process messages.");
    return false;
}

#endif