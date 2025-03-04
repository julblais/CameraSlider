#ifndef NETAPP_H
#define NETAPP_H

#include "appConfig.h"
#include "src/core/time/timer.h"
#include "src/core/app/appBase.h"
#include "src/core/network/address.h"

enum ConnectionState
{
    BROADCASTING,
    WAITING_FOR_CONNECTION,
    SENDING_HANDSHAKE,
    SENDING_REQUEST,
    WAITING_FOR_HANDSHAKE,
    CONNECTED
};

class BrainApp : public Core::AppBase
{
public:
    BrainApp(const Slider::AppConfig& config);
    virtual void Setup() override;
    virtual void Update() override;

private:
    ConnectionState state;
    bool isComplete;
    Core::MacAddress controllerMac;
    Core::Timer m_Timeout;

};

class ControllerApp : public Core::AppBase
{
public:
    ControllerApp(const Slider::AppConfig& config);
    virtual void Setup() override;
    virtual void Update() override;

private:
    ConnectionState state;
    bool isComplete;
    Core::MacAddress brainMac;
    Core::Timer m_Timeout;
};

#endif