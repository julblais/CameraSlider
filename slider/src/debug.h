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
#define LogError(...) {Debug::LogStyle(Debug::Styler(Debug::Red), "Error\t");\
Debug::Log(__FILE__, "(", __LINE__, "): ",  __VA_ARGS__);}
#else
#define LogError(...) ;
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#define LogWarning(...) {Debug::LogStyle(Debug::Styler(Debug::Yellow), "Warning\t");\
Debug::Log(__VA_ARGS__);}
#else
#define LogWarning(...) ;
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LogInfo(...) {Debug::LogStyle(Debug::Styler(Debug::Cyan), "Info\t");\
Debug::Log(__VA_ARGS__);}
#else
#define LogInfo(...) ;
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LogDebug(...) {Debug::LogStyle(Debug::Styler(Debug::White), "Debug\t");\
Debug::Log(__VA_ARGS__);}
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

    constexpr Color Black = Color::Black;
    constexpr Color Red = Color::Red;
    constexpr Color Green = Color::Green;
    constexpr Color Yellow = Color::Yellow;
    constexpr Color Blue = Color::Blue;
    constexpr Color Purple = Color::Purple;
    constexpr Color Cyan = Color::Cyan;
    constexpr Color White = Color::White;

    constexpr Style Bold = Style::Bold;
    constexpr Style Italic = Style::Italic;
    constexpr Style Underline = Style::Underline;
    constexpr Style Strikethrough = Style::Strikethrough;

    struct Styler
    {
        constexpr Styler(Color color = Color::None, Style style = Style::Bold, Color background = Color::None)
            :color(color), style(style), background(background)
        {}
        constexpr Styler(Style style)
            : Styler(Color::None, style)
        {}
        Color color;
        Style style;
        Color background;
    };

    template <typename... TArgs>
    void Log(TArgs&&... args)
    {
        Core::PassParamPack { (Serial.print(args), 1)... };
        Serial.println();
    }

    template <typename... TArgs>
    void LogStyle(const Styler& styler, TArgs&&... args)
    {
        constexpr const char* RESET = "\e[0m";

        if (styler.style != Style::Regular)
            Serial.printf("\e[%im", static_cast<int>(styler.style));
        if (styler.color != Color::None)
            Serial.printf("\e[3%im", static_cast<int>(styler.color));
        if (styler.background != Color::None)
            Serial.printf("\e[4%im", static_cast<int>(styler.background));

        Core::PassParamPack { (Serial.print(args), 1)... };
        Serial.print(RESET);
    }

    void Init(const int baud);
}

#endif