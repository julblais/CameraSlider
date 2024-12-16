#include "app.h"
#include "menu.h"
#include "lcd.h"
#include "dpad.h"
#include "debug.h"

static Rig::AppConfig CreateConfig()
{
    Rig::AppConfig config = Rig::AppConfig();
    config.DpadHorizontalPin    = A19;
    config.DpadVerticalPin      = A18;
    config.DpadSelectionPin     = T8;
    config.LcdAddress           = 0x27;
    return config;
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
}