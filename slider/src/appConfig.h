#ifndef APPCONFIG_H
#define APPCONFIG_H

namespace Slider
{
    struct AppConfig
    {
        unsigned char DpadUpPin;
        unsigned char DpadDownPin;
        unsigned char DpadLeftPin;
        unsigned char DpadRightPin;
        unsigned char DpadSelectionPin;
        unsigned char JoystickXPin;
        unsigned char JoystickYPin;
        unsigned char JoystickCenterPin;
        unsigned char StepperDirectionPin;
        unsigned char StepperStepPin;
        int LcdAddress;
        int ShowMenuDelayMs;
    };
}

#endif