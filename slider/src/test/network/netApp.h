#ifndef NETAPP_H
#define NETAPP_H

#include "src/app/appBase.h"
#include "src/app/appConfig.h"
#include "src/test/network/address.h"

class NetApp : public Slider::AppBase
{
    public:
        NetApp(const Slider::AppConfig &config);
        virtual void Setup() override;
        virtual void Update() override;

        void UpdateController();
        void UpdateReceiver();

    private:
        bool isReceiver;
        bool isConnected;
        bool hasHandshake;
        bool isCompleted;
        Network::MacAddress otherMac;
        //bool isConnected;
};

#endif