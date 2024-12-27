#ifndef DBG_H
#define DBG_H

#define USE_SERIAL 1

#include "src/utils/templateUtils.h"

#ifdef USE_SERIAL
    #include <HardwareSerial.h>
#endif

class Debugger
{
    public:
        Debugger() {}

        void Init(const int baud)
        {
            #if USE_SERIAL
                Serial.begin(baud);
            #endif
        }

        template <typename... TArgs>
        void Log(TArgs&&... args)
        {
            #if USE_SERIAL
                PassParamPack{(Serial.print(args), 1)...};
                Serial.println();
            #endif
        }
};

static Debugger Debug = Debugger();

#endif