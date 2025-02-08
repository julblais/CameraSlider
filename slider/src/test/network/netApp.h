#ifndef NETAPP_H
#define NETAPP_H

#include "src/app/appBase.h"
#include "src/app/appConfig.h"
#include "src/test/network/address.h"

    enum ConnectionState
    {
        BROADCASTING,
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
        std::atomic_flag fl;
        ConnectionState state;
        bool isComplete;
        Network::MacAddress controllerMac;
};

class ControllerApp : public Slider::AppBase
{
    public:
        ControllerApp(const Slider::AppConfig &config);
        virtual void Setup() override;
        virtual void Update() override;

    private:
        bool isConnected;
        bool hasHandshake;
        bool isCompleted;
        bool hasSentHandshake;
        Network::MacAddress brainMac;
};

#endif