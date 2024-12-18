#include "app/app.h"
#include "debug.h"

static Rig::AppConfig CreateConfig()
{
    Rig::AppConfig conf = Rig::AppConfig();
    conf.DpadHorizontalPin    = A19;
    conf.DpadVerticalPin      = A18;
    conf.DpadSelectionPin     = T8;
    conf.LcdAddress           = 0x27;
    conf.isSimulator          = true;
    conf.showMenuDelayMs      = 2000;
    return conf;
}

static Rig::AppConfig config = CreateConfig();
static Rig::App app(config);

void setup() {
    Debug.Init(9600);
    Debug.Log("Being setup...");

    app.Setup();

    Debug.Log("End setup.");
}

void loop() {
    app.Update();

    if (config.isSimulator) //somehow this makes the timing more accurate...
        delay(10);
}