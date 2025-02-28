#ifndef DBG_H
#define DBG_H

#include "src/core/utils/templateUtils.h"
#include "Print.h"
#include <HardwareSerial.h>
#include "log.h"

#define LOG_LEVEL_NONE 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_DEBUG 4

#define LOG_LEVEL LOG_LEVEL_INFO

//-------------------------------------

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LogError(...) Debug::LoggerStyle(Debug::Bold, Debug::Color::Red).Log("Error\t");\
Debug::Logger().Log(" ", __FILE__, "(", __LINE__, "): ",  __VA_ARGS__)
#else
#define LogError(...) ;
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#define LogWarning(...) Debug::LoggerStyle(Debug::Regular, Debug::Color::Yellow).Log("Warning\t");\
Debug::Logger().Log(" ", __VA_ARGS__)
#else
#define LogWarning(...) ;
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LogInfo(...) Debug::LoggerStyle(Debug::Regular, Debug::Color::Cyan).Log("Info\t");\
Debug::Logger().Log(" ", __VA_ARGS__)
#else
#define LogInfo(...) ;
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LogDebug(...) Debug::LoggerStyle(Debug::Regular, Debug::Color::White).Log("Debug\t");\
Debug::Logger().Log(" ", __VA_ARGS__)
#else
#define LogDebug(...) ;
#endif

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
        White = 7
    };

    enum class Style : char
    {
        Regular = 0,
        Bold = 1,
        Italic = 3,
        Underline = 4,
        Strikethrough = 9
    };

    constexpr Color Black = Color::Black;
    constexpr Style Regular = Style::Regular;
    constexpr Style Bold = Style::Bold;

    //idea : eLog::Colorize::colorize("string", eLog::AsciiColor::ColorEnum::BOLD_RED, true)
    //as a function???


    struct LoggerStyle
    {
        static constexpr const char* RESET = "\e[0m";

        static void Colorize()
        {
            
        }

        LoggerStyle(Style style = Style::Regular, Color color = Black, Color background = Black)
        {
            m_color = color;
            m_style = style;
            m_back = background;
        }

        template <typename... TArgs>
        void Log(TArgs&&... args)
        {
            Serial.printf("\e[%i;3%i;4%im", static_cast<int>(m_style), static_cast<int>(m_color), static_cast<int>(m_back));
            Core::PassParamPack { (Serial.print(args), 1)... };
            Serial.print(RESET);
        }

        Color m_color;
        Color m_back;
        Style m_style;
    };

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