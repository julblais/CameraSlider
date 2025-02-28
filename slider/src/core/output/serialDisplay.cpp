#include "serialDisplay.h"
#include "HardwareSerial.h"
#include "src/core/time/timer.h"

using namespace Core;

void SerialDisplay::Init()
{
    m_Timer = Timer::Create("SerialDisplay", [this]() {
        PrintToSerial();
    });
    m_Timer.Start(300, true);
}

size_t Core::SerialDisplay::write(uint8_t value)
{
    //do not go over the line!
    const auto maxCursor = ((m_Cursor / LCD_LINE_LENGTH) + 1) * LCD_LINE_LENGTH;
    if (m_Cursor < maxCursor)
        m_Buffer[m_Cursor++] = value;
    return 1;
}

void SerialDisplay::SetCursor(const int column, const int row)
{
    if (row >= LCD_NUM_LINES || row < 0)
        return;
    if (column >= LCD_LINE_LENGTH || column < 0)
        return;
    m_Cursor = row * LCD_LINE_LENGTH + column;
}

SymbolHandle SerialDisplay::GetSymbol(Symbol symbol) const
{
    return SymbolHandle('|');
}

void SerialDisplay::Clear()
{
    m_Cursor = 0;
    m_Buffer.fill(' ');
}

void SerialDisplay::FillCurrentLine()
{
    const auto maxCursor = ((m_Cursor / LCD_LINE_LENGTH) + 1) * LCD_LINE_LENGTH;
    while (m_Cursor < maxCursor)
        m_Buffer[m_Cursor++] = ' ';
}

void PrintBorder(const char* text)
{
    constexpr const char* border = "\e[48;5;22m";
    constexpr const char* reset = "\e[0m";

    Serial.print(border);
    Serial.print(text);
    Serial.print(reset);
}

void PrintBorderln(const char* text)
{
    PrintBorder(text);
    Serial.println();
}

void SerialDisplay::PrintToSerial() const
{
    constexpr const char* format = "\e[48;5;112m";
    constexpr const char* goBackUp = "\r\e[4F";
    constexpr const char* reset = "\e[0m";
    constexpr const char* hborder = "                    ";
    constexpr const char* vborder = "  ";

    const auto areEqual = std::equal(
        std::begin(m_Buffer), std::end(m_Buffer), std::begin(m_PreviousBuffer));

    if (!areEqual)
    {
        unsigned int count = 0;
        unsigned int line = 0;

        PrintBorderln(hborder);
        PrintBorder(vborder);
        Serial.print(format);
        for (const auto it : m_Buffer)
        {
            if (count >= LCD_LINE_LENGTH)
            {
                Serial.print(reset);
                PrintBorderln(vborder);
                PrintBorder(vborder);
                Serial.print(format);
                count = 0;
            }
            count++;
            Serial.write(it);
        }
        Serial.print(reset);
        PrintBorderln(vborder);
        PrintBorderln(hborder);
        Serial.print(goBackUp);
        std::copy(std::begin(m_Buffer), std::end(m_Buffer), std::begin(m_PreviousBuffer));
    }
}
