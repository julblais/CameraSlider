#ifndef CONNECTOR_BRAIN_H
#define CONNECTOR_BRAIN_H

#include "core/component.h"
#include "core/messageCallbackHandle.h"
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
