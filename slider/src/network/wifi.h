#ifndef WIFI_H
#define WIFI_H

#include "src/component/component.h"

namespace Net
{
    class Wifi : public Slider::Component
    {
        public:
            virtual void Setup() override;
            virtual void Update() override;
    };
}

#endif