#ifndef DBG_H
#define DBG_H

#define USE_SERIAL 1

#include "src/utils/templateUtils.h"

#ifdef USE_SERIAL
    #include <HardwareSerial.h>
#endif

namespace Debug
{
    void Init(const int baud);

    template <typename... TArgs>
    void Log(TArgs&&... args);
}

template <typename... TArgs>
void Debug::Log(TArgs&&... args)
{
    #if USE_SERIAL
        PassParamPack{(Serial.print(args), 1)...};
        Serial.println();
    #endif
}

#endif