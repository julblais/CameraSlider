#include "displayBuffer.h"

Output::DisplayBuffer::DisplayBuffer():
    m_Cursor(0), m_Buffer()
{
    m_Buffer.fill(' ');
}

void Output::DisplayBuffer::fillCurrentLine()
{
    const auto maxCursor = ((m_Cursor / LCD_LINE_LENGTH) + 1) * LCD_LINE_LENGTH;
    while (m_Cursor < maxCursor)
        m_Buffer[m_Cursor++] = ' ';
}

size_t Output::DisplayBuffer::write(Keycode value)
{
    //do not go over the line!
    const auto maxCursor = ((m_Cursor / LCD_LINE_LENGTH) + 1) * LCD_LINE_LENGTH;
    if (m_Cursor < maxCursor)
        m_Buffer[m_Cursor++] = value;
    return 1;
}

void Output::DisplayBuffer::SetCursor(const int line, const int column)
{
    if (line >= LCD_NUM_LINES || line < 0)
        return;
    if (column >= LCD_LINE_LENGTH || column < 0)
        return;
    m_Cursor = line * LCD_LINE_LENGTH + column;
}
