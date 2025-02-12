#ifndef WIFI_H
#define WIFI_H

#include "src/component/component.h"

namespace Net
{
    class WifiComponent : public Slider::Component
    {
        public:
            void OnReceive(const uint8_t* mac_addr, const uint8_t *data, size_t length);
            virtual void Setup() override;
            virtual void Update() override;
    };
}

#endif