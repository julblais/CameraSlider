#ifndef WIFICOMPONENT_H
#define WIFICOMPONENT_H

#ifdef USE_NETWORK

#include "src/core/component/component.h"

using namespace Core;

namespace Net
{
    class WifiComponent : public Component
    {
    public:
        void Setup() override;
        void Update() override;
    };
}

#endif
#endif
