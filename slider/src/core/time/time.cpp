#include "time.h"
#include <esp32-hal-timer.h>

Core::Time Core::GetTimeMs()
{
    return millis();
}
