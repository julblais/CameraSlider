#include "displayBuffer.h"
#include "src/output/display.h"

Output::DisplayBuffer::DisplayBuffer()
{
    m_Buffer.fill(' ');
    m_PreviousBuffer.fill(' ');
    m_Cursor = 0;
}

void Output::DisplayBuffer::FillCurrentLine()
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

void Output::DisplayBuffer::Init(Display *display)
{
    m_Display = display;
}

void Output::DisplayBuffer::Clear()
{
    m_Cursor = 0;
    m_Buffer.fill(' ');
}

void Output::DisplayBuffer::SetCursor(const int line, const int column)
{
    if (line >= LCD_NUM_LINES || line < 0)
        return;
    if (column >= LCD_LINE_LENGTH || column < 0)
        return;
    m_Cursor = line * LCD_LINE_LENGTH + column;
}

void Output::DisplayBuffer::PrintToDisplay() const
{
    const auto areEqual = std::equal(
        std::begin(m_Buffer), std::end(m_Buffer), std::begin(m_PreviousBuffer));

    if (!areEqual)
    {
        m_Display->SetCursor(0, 0);
        unsigned int count = 0;
        unsigned int line = 0;
        for (const auto it : *this)
        {
            if (count >= LCD_LINE_LENGTH)
            {
                m_Display->SetCursor(0, ++line);
                count = 0;
            }
            count++;
            m_Display->Write(it);
        }
        std::copy(std::begin(m_Buffer), std::end(m_Buffer), std::begin(m_PreviousBuffer));
    }
}
