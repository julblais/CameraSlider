#ifndef SLIDER_BLUETOOTH_COMPONENT_H
#define SLIDER_BLUETOOTH_COMPONENT_H

#if !IS_SIMULATOR

#include "core/component.h"
#include "core/address.h"

namespace Bt
{
    class BluetoothGamepad;

    class BluetoothComponent : public Core::Component
    {
    public:
        void Setup() override;
        void Update() override;

        BluetoothGamepad* GetGamepad();
        void DisconnectGamepad();
        void EnablePairing();
        void DisablePairing();
        bool IsPairing() const;
        void Reset();
        Core::MacAddress GetMacAddress() const;

    private:
        bool m_IsPairing = false;
    };
}

#endif
#endif
