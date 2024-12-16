#ifndef DBG_H
#define DBG_H

#define USE_SERIAL 1

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

        template<typename... T>
        void Log(T&&... args)
        {
            #if USE_SERIAL
                for(auto &&arg: { args... }) {
                    Serial.print(arg);
                }
                Serial.println("");
            #endif
        }
};

static Debugger Debug = Debugger();

#endif
