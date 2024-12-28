#include "app/app.h"
#include "debug.h"

#ifdef IS_SIMULATOR
    #define C_DpadUpPin             23;
    #define C_DpadDownPin           14;
    #define C_DpadLeftPin           13;
    #define C_DpadRightPin          19;
    #define C_DpadSelectionPin      18;
    #define C_JoystickXPin          26;
    #define C_JoystickYPin          25;
    #define C_JoystickCenterPin     5;
    #define C_StepperStepPin        33;
    #define C_StepperDirectionPin   32;
    #define C_LcdAddress        0x27;
    #define C_ShowMenuDelayMs   2000;
#else
    #include "slider.ino"
#endif

static Slider::AppConfig CreateConfig()
{
    auto conf = Slider::AppConfig();
    conf.DpadUpPin           = C_DpadUpPin;
    conf.DpadDownPin         = C_DpadDownPin;
    conf.DpadLeftPin         = C_DpadLeftPin;
    conf.DpadRightPin        = C_DpadRightPin;
    conf.DpadSelectionPin    = C_DpadSelectionPin;
    conf.JoystickXPin        = C_JoystickXPin;
    conf.JoystickYPin        = C_JoystickYPin;
    conf.JoystickCenterPin   = C_JoystickCenterPin;
    conf.StepperStepPin      = C_StepperStepPin;
    conf.StepperDirectionPin = C_StepperDirectionPin;
    conf.LcdAddress          = C_LcdAddress;
    conf.ShowMenuDelayMs     = C_ShowMenuDelayMs;
    return conf;
}

static Slider::App app(CreateConfig());

void setup() {
    Debug::Init(9600);
    Debug::Log("Being setup...");

    app.Setup();

    Debug::Log("End setup.");
}

void loop() {
    app.Update();

    #ifdef IS_SIMULATOR //somehow this makes the timing more accurate...
        delay(10);
    #endif
}