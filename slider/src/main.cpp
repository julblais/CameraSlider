#include "app/app.h"
#include "debug.h"

#ifdef IS_SIMULATOR
    #define C_DpadHorizontalPin A19;
    #define C_DpadVerticalPin   A18;
    #define C_DpadSelectionPin  T8;
    #define C_LcdAddress        0x27;
    #define C_ShowMenuDelayMs   2000;
#else
    #include "slider.ino"
#endif

static Slider::AppConfig CreateConfig()
{
    auto conf = Slider::AppConfig();
    conf.DpadHorizontalPin    = C_DpadHorizontalPin;
    conf.DpadVerticalPin      = C_DpadVerticalPin;
    conf.DpadSelectionPin     = C_DpadSelectionPin;
    conf.LcdAddress           = C_LcdAddress;
    conf.ShowMenuDelayMs      = C_ShowMenuDelayMs;
    return conf;
}

static Slider::App app(CreateConfig());

void setup() {
    Debug.Init(9600);
    Debug.Log("Being setup...");

    app.Setup();

    Debug.Log("End setup.");
}

void loop() {
    app.Update();

    #ifdef IS_SIMULATOR //somehow this makes the timing more accurate...
        delay(10);
    #endif
}