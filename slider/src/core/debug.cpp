#include "debug.h"

Debug::Styler::Styler(Color color, Style style, Color background)
{
    if (style != Style::Regular)
        Serial.printf("\e[%im", static_cast<int>(style));
    if (color != Color::None)
        Serial.printf("\e[3%im", static_cast<int>(color));
    if (background != Color::None)
        Serial.printf("\e[4%im", static_cast<int>(background));
}

Debug::Styler::Styler(Style style) : Styler(Color::None, style) {}

Debug::Styler::~Styler()
{
    constexpr const char* RESET = "\e[0m";
    Serial.print(RESET);
}

void Debug::Init(const int baud)
{
    Serial.begin(baud);
}
