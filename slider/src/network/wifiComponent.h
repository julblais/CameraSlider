#ifndef WIFICOMPONENT_H
#define WIFICOMPONENT_H

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
