#include "time.h"
#include <esp32-hal-timer.h>

using namespace Core;

Time Core::GetTimeMs()
{
    return millis();
}
