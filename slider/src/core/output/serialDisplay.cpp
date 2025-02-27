#include "serialDisplay.h"
#include "HardwareSerial.h"
#include "src/core/time/timer.h"

using namespace Core;

void SerialDisplay::Init()
{
    m_Timer = Timer::Create("SerialDisplay", [this]() {
        PrintToSerial();
    });
    m_Timer.Start(1000, true);
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
    return SymbolHandle('?');
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

void SerialDisplay::PrintToSerial() const
{
    const auto areEqual = std::equal(
        std::begin(m_Buffer), std::end(m_Buffer), std::begin(m_PreviousBuffer));

    if (!areEqual)
    {
        Serial.println("-----------------");
        unsigned int count = 0;
        unsigned int line = 0;
        for (const auto it : m_Buffer)
        {
            if (count >= LCD_LINE_LENGTH)
            {
                Serial.println();
                count = 0;
            }
            count++;
            Serial.write(it);
        }
        Serial.println();
        Serial.println("-----------------");
        std::copy(std::begin(m_Buffer), std::end(m_Buffer), std::begin(m_PreviousBuffer));
    }
}
