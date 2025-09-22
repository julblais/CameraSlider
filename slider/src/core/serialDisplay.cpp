#include "serialDisplay.h"
#include "HardwareSerial.h"
#include "timer.h"

using namespace Core;

SerialDisplay::SerialDisplay()
    : m_Timer(Timer::Create("SerialDisplay", [this]() { PrintToSerial(); })),
      m_Cursor(0)
{
    m_Buffer.fill(' ');
    m_PreviousBuffer.fill('-');
    m_Timer.Start(300, true);
}

size_t SerialDisplay::write(const uint8_t value)
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

#ifdef USE_ADVANCED_SERIAL

constexpr const char* border = "\e[48;5;22m";
constexpr const char* reset = "\e[0m";
constexpr const char* format = "\e[1m\e[30m\e[48;5;112m";
constexpr const char* goBackUp = "\r\e[4F";
constexpr const char* hborder = "                    ";
constexpr const char* vborder = "  ";

void PrintBorder(const char* text)
{
    Serial.print(border);
    Serial.print(text);
    Serial.print(reset);
}

void PrintBorderln(const char* text)
{
    PrintBorder(text);
    Serial.println();
}

void WriteToDisplay(const uint8_t buffer[], const size_t length, const size_t lineLength)
{
    unsigned int count = 0;

    PrintBorderln(hborder);
    PrintBorder(vborder);
    Serial.print(format);

    for (int i = 0; i < length; i++)
    {
        if (count >= lineLength)
        {
            Serial.print(reset);
            PrintBorderln(vborder);
            PrintBorder(vborder);
            Serial.print(format);
            count = 0;
        }
        count++;
        Serial.write(buffer[i]);
    }

    Serial.print(reset);
    PrintBorderln(vborder);
    PrintBorderln(hborder);
    Serial.print(goBackUp);
}
#else
void WriteToDisplay(const uint8_t buffer[], const size_t length, const size_t lineLength)
{
    unsigned int count = 0;
    for (int i = 0; i < length; i++)
    {
        if (count >= lineLength)
        {
            Serial.println();
            count = 0;
        }
        Serial.write(buffer[i]);
        count++;
    }
    Serial.println();
}
#endif

void SerialDisplay::PrintToSerial() const
{
    const auto areEqual = std::equal(
        std::begin(m_Buffer), std::end(m_Buffer), std::begin(m_PreviousBuffer));

    if (!areEqual)
    {
        WriteToDisplay(m_Buffer.data(), m_Buffer.size(), LCD_LINE_LENGTH);
        std::copy(std::begin(m_Buffer), std::end(m_Buffer), std::begin(m_PreviousBuffer));
    }
}
