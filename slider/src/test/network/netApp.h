#ifndef NETAPP_H
#define NETAPP_H

#include "src/app/appBase.h"
#include "src/app/appConfig.h"

class NetApp : public Slider::AppBase
{
    public:
        NetApp(const Slider::AppConfig &config);
        virtual void Setup() override;
        virtual void Update() override;
};

#endif