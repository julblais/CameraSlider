#ifndef WIFICOMPONENT_H
#define WIFICOMPONENT_H

#include "src/core/component/component.h"

using namespace Core;

namespace Net
{
    class WifiComponent : public Component
    {
    public:
        virtual void Setup() override;
        virtual void Update() override;
    };
}

#endif