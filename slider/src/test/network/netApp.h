#ifndef NETAPP_H
#define NETAPP_H

#include "src/app/appBase.h"
#include "src/app/appConfig.h"

struct HandshakeMessage
{
    uint8_t mac[6];
};

struct InputMessage
{
    int x;
    int y;
};

class NetApp : public Slider::AppBase
{
    public:
        NetApp(const Slider::AppConfig &config);
        virtual void Setup() override;
        virtual void Update() override;

        void UpdateSender();
        void UpdateReceiver();
        
        void Sender__ReceiveMsg(HandshakeMessage message);
        void Sender__ReceiveMsg(InputMessage message);

    private:
        bool isReceiver;
        //bool isConnected;
};

#endif