#include "debug.h"


void Debug::Init(const int baud)
{
    Serial.begin(baud);
}