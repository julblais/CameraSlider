#ifndef SLIDER_BLUETOOTH_COMPONENT_H
#define SLIDER_BLUETOOTH_COMPONENT_H

#if !IS_SIMULATOR

#include "bluetoothGamepad.h"
#include "core/component.h"
#include "core/address.h"

class Controller;

namespace Bt
{
    class BluetoothComponent : Core::Component
    {
    public:
        void Setup() override;
        void Update() override;

        BluetoothGamepad GetGamepad() const;
        void DisconnectController();
        void EnablePairing();
        void DisablePairing();
        void Reset();
        Core::MacAddress GetMacAddress() const;
    };
}

#endif
#endif
