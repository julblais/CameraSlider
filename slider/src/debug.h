#ifndef DBG_H
#define DBG_H

#include "src/core/utils/templateUtils.h"

#include <HardwareSerial.h>

#define LOG_LEVEL_NONE 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_DEBUG 4

#define LOG_LEVEL LOG_LEVEL_INFO

//-------------------------------------

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LogError(...) Debug::Logger().Log(__FILE__, "(", __LINE__, "): ",  __VA_ARGS__)
#else
#define LogError(...) ;
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#define LogWarning(...) Debug::Logger().Log(__VA_ARGS__)
#else
#define LogWarning(...) ;
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LogInfo(...) Debug::Logger().Log(__VA_ARGS__)
#else
#define LogInfo(...) ;
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LogDebug(...) Debug::Logger().Log(__VA_ARGS__)
#else
#define LogDebug(...) ;
#endif

namespace Debug
{
    struct Logger
    {
        template <typename... TArgs>
        void Log(TArgs&&... args)
        {
            Core::PassParamPack { (Serial.print(args), 1)... };
        }

        ~Logger()
        {
            Serial.println();
        }
    };

    void Init(const int baud);
}

#endif