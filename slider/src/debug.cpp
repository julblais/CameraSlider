#include "debug.h"


void Debug::Init(const int baud)
{
    #if USE_SERIAL
        Serial.begin(baud);
    #endif
}