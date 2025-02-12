#ifndef NETAPP_H
#define NETAPP_H

#include "src/app/appBase.h"
#include "src/app/appConfig.h"
#include "src/test/network/address.h"

    enum ConnectionState
    {
        BROADCASTING,
        WAITING_FOR_CONNECTION,
        SENDING_HANDSHAKE,
        SENDING_REQUEST,
        WAITING_FOR_HANDSHAKE,
        CONNECTED
    };

class BrainApp : public Slider::AppBase
{
    public:
        BrainApp(const Slider::AppConfig &config);
        virtual void Setup() override;
        virtual void Update() override;

    private:
        ConnectionState state;
        bool isComplete;
        Net::MacAddress controllerMac;
};

class ControllerApp : public Slider::AppBase
{
    public:
        ControllerApp(const Slider::AppConfig &config);
        virtual void Setup() override;
        virtual void Update() override;

    private:
        ConnectionState state;
        bool isComplete;
        Net::MacAddress brainMac;
};

#endif