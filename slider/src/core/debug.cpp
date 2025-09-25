#include "debug.h"

using namespace Debug;

Styler::Styler(Color color, Style style, Color background)
{
    if (style != Style::Regular)
        Serial.printf("\e[%im", static_cast<int>(style));
    if (color != Color::None)
        Serial.printf("\e[3%im", static_cast<int>(color));
    if (background != Color::None)
        Serial.printf("\e[4%im", static_cast<int>(background));
}

Styler::Styler(Style style) : Styler(Color::None, style) {}

Styler::~Styler()
{
    constexpr const char* RESET = "\e[0m";
    Serial.print(RESET);
}

void PrintInternal(const char* msg, const Color color = Color::None, const Style style = Style::Bold,
                   const Color background = Color::None)
{
    Styler styler(color, style, background);
    Serial.print(msg);
}

void Debug::PrintError()
{
    PrintInternal("Error\t", Color::Red);
}

void Debug::PrintWarning()
{
    PrintInternal("Warning\t", Color::Yellow);
}

void Debug::PrintInfo()
{
    PrintInternal("Info\t", Color::Cyan);
}

void Debug::PrintDebug()
{
    PrintInternal("Debug\t", Color::White);
}

void Debug::Init(const int baud)
{
    Serial.begin(baud);
}
