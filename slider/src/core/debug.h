#ifndef DBG_H
#define DBG_H

#include "utils/templateUtils.h"
#include <HardwareSerial.h>

#define LOG_LEVEL_NONE 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_DEBUG 4

#define LOG_LEVEL LOG_LEVEL_INFO

namespace Debug
{
    enum class Color : char
    {
        Black = 0,
        Red = 1,
        Green = 2,
        Yellow = 3,
        Blue = 4,
        Purple = 5,
        Cyan = 6,
        White = 7,
        None = 8  //yeah I know
    };

    enum class Style : char
    {
        Regular = 0,
        Bold = 1,
        Italic = 3,
        Underline = 4,
        Strikethrough = 9
    };

    struct Styler
    {
        Styler(Color color = Color::None, Style style = Style::Bold, Color background = Color::None);
        Styler(Style style);
        ~Styler();
    };

    void PrintError();
    void PrintWarning();
    void PrintInfo();
    void PrintDebug();

    template <typename... TArgs>
    void Log(TArgs&&... args)
    {
        Core::PassParamPack { (Serial.print(args), 1)... };
        Serial.println();
    }

    void Init(const int baud);
}

//-------------------------------------

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LogError(...) { Debug::PrintError(); \
Debug::Log(__FILE__, "(", __LINE__, "): ",  __VA_ARGS__);}
#else
#define LogError(...) ;
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#define LogWarning(...) { Debug::PrintWarning(); \
Debug::Log(__VA_ARGS__);}
#else
#define LogWarning(...) ;
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LogInfo(...) { Debug::PrintInfo(); \
Debug::Log(__VA_ARGS__);}
#else
#define LogInfo(...) ;
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LogDebug(...) { Debug::PrintInfo(); \
Debug::Log(__VA_ARGS__);}
#else
#define LogDebug(...) ;
#endif


#endif
