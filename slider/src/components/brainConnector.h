#ifndef CONNECTOR_BRAIN_H
#define CONNECTOR_BRAIN_H

#include "src/core/component/component.h"
#include "src/core/network/messageCallbackHandle.h"
#include "src/core/time/timer.h"
#include <vector>

namespace Slider
{
    class BrainConnector : public Core::Component
    {
    public:
        BrainConnector();

        void Setup() override;
        void Update() override;

    private:
        std::vector<Core::MessageCallbackHandle> m_Callbacks;
        bool m_BroadcastActive;
    };
}

#endif
