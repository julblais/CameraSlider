#ifdef IS_SIMULATOR
#define C_DpadUpPin             23
#define C_DpadDownPin           34
#define C_DpadLeftPin           16
#define C_DpadRightPin          19
#define C_DpadSelectionPin      18
#define C_JoystickXPin          35
#define C_JoystickYPin          39
#define C_JoystickCenterPin     17
#define C_StepperStepPin        33
#define C_StepperDirectionPin   32
#define C_LcdAddress        0x27
#define C_ShowMenuDelayMs   2000
#else
#include "slider.ino"
#endif

#include "src/core/app/appBase.h"
#include "src/app/appConfig.h"
#include "debug.h"
#include "src/core/perf/perf.h"
#include <memory>

using namespace Slider;

static AppConfig CreateConfig()
{
    auto conf = AppConfig();
    conf.DpadUpPin = C_DpadUpPin;
    conf.DpadDownPin = C_DpadDownPin;
    conf.DpadLeftPin = C_DpadLeftPin;
    conf.DpadRightPin = C_DpadRightPin;
    conf.DpadSelectionPin = C_DpadSelectionPin;
    conf.JoystickXPin = C_JoystickXPin;
    conf.JoystickYPin = C_JoystickYPin;
    conf.JoystickCenterPin = C_JoystickCenterPin;
    conf.StepperStepPin = C_StepperStepPin;
    conf.StepperDirectionPin = C_StepperDirectionPin;
    conf.LcdAddress = C_LcdAddress;
    conf.ShowMenuDelayMs = C_ShowMenuDelayMs;
    return conf;
}

static std::unique_ptr<AppBase> app;

void setup()
{
    Debug::Init(9600);
    INIT_SAMPLER(CpuSampler);
    INIT_SAMPLER(MemorySampler);

    LogInfo("Being setup...");
    app = Core::AppCreator<AppConfig>::Create(CreateConfig());
    app->Setup();
    LogInfo("End setup.");
}

void loop()
{
    MEASURE("AppUpdate", []() {
        app->Update();
    }, CpuSampler, MemorySampler);

    #ifdef IS_SIMULATOR //somehow this makes the timing more accurate...
    delay(30);
    #endif
}