#ifndef APP_BASE_H
#define APP_BASE_H

#include <memory>

namespace Slider
{
    struct AppConfig
    {
        public:
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
    
    class AppBase
    {
        public:
            virtual void Setup() = 0;
            virtual void Update() = 0;
    };

    class AppCreator
    {
        public:
            static std::unique_ptr<AppBase> Create(const AppConfig& config);
    };
}

#endif