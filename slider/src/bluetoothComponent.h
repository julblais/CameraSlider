#ifndef SLIDER_BLUETOOTH_COMPONENT_H
#define SLIDER_BLUETOOTH_COMPONENT_H

#if !IS_SIMULATOR

#include "core/component.h"
#include "core/address.h"

namespace Slider
{
    class BluetoothComponent : Core::Component
    {
    public:
        void Setup() override;
        void Update() override;

        void DisconnectController();
        void EnablePairing();
        void DisablePairing();
        void Reset();
        Core::MacAddress GetMacAddress() const;
    };
}

#endif
#endif
