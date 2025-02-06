#ifndef NETAPP_H
#define NETAPP_H

#include "src/app/appBase.h"
#include "src/app/appConfig.h"
#include "src/test/network/address.h"

class BrainApp : public Slider::AppBase
{
    public:
        BrainApp(const Slider::AppConfig &config);
        virtual void Setup() override;
        virtual void Update() override;

    private:
        bool isConnected;
        bool isCompleted;
        bool hasSentHandshake;
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