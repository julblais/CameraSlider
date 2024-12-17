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

        template <typename T>
        void Log(T&& t) 
        {
            #if USE_SERIAL
                Serial.println(t);
            #endif
        }

        template <typename T1, typename T2>
        void Log(T1&& t1, T2&& t2) 
        {
            #if USE_SERIAL
                Serial.print(t1);
                Log(t2);
            #endif
        }

        template <typename T1, typename T2, typename T3>
        void Log(T1&& t1, T2&& t2, T3&& t3) 
        {
            #if USE_SERIAL
                Serial.print(t1);
                Log(t2, t3);
            #endif
        }

        template <typename T1, typename T2, typename T3, typename T4>
        void Log(T1&& t1, T2&& t2, T3&& t3, T4&& t4) 
        {
            #if USE_SERIAL
                Serial.print(t1);
                Log(t2, t3, t4);
            #endif
        }
};

static Debugger Debug = Debugger();

#endif
