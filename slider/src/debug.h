#ifndef DBG_H
#define DBG_H

#include "src/utils/templateUtils.h"

#include <HardwareSerial.h>

#define ENABLE_LOG 1

#if ENABLE_LOG
#define CLOG(val) Serial.print(__FILE__);Serial.print(val)
#else
#define CLOG(val)
#endif

namespace Debug
{
    enum class Level : char
    {
        ERROR,
        WARNING,
        INFO,
        DEBUG
    };

    struct Logger 
    {
        template <typename T>
        Logger& operator<<(T&& value)
        {
            CLOG(value);
            return *this;
        }

        ~Logger() 
        {
            Serial.println();
        }
    };

    void Init(const int baud);
}

extern Debug::Level loglevel;

#define Log(level) if (level > loglevel) ; \
    else Debug::Logger()
    
#define LogError if (Debug::Level::ERROR > loglevel) ; \
    else Debug::Logger()
    
#define LogWarn if (Debug::Level::WARNING > loglevel) ; \
    else Debug::Logger()
    
#define LogInfo if (Debug::Level::INFO > loglevel) ; \
    else Debug::Logger()

#define LogDebug if (Debug::Level::DEBUG > loglevel) ; \
    else Debug::Logger()



#endif