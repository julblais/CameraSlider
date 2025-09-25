#include "displayBuffer.h"

IO::DisplayBuffer::DisplayBuffer() :
    m_Display(nullptr)
{
    m_Buffer.fill(' ');
    m_PreviousBuffer.fill(' ');
    m_Cursor = 0;
}

void IO::DisplayBuffer::FillCurrentLine()
{
    const auto maxCursor = (m_Cursor / LCD_LINE_LENGTH + 1) * LCD_LINE_LENGTH;
    while (m_Cursor < maxCursor)
        m_Buffer[m_Cursor++] = ' ';
}

size_t IO::DisplayBuffer::write(uint8_t value)
{
    switch (value)
    {
    case '\n':
        {
            FillCurrentLine();
            return 0;
        }
    case '\r':
    case '\t':
        {
            return 0;
        }
    default:
        {
            //do not go over the line!
            const auto maxCursor = (m_Cursor / LCD_LINE_LENGTH + 1) * LCD_LINE_LENGTH;
            if (m_Cursor < maxCursor)
                m_Buffer[m_Cursor++] = value;
            return 1;
        }
    }
}

void IO::DisplayBuffer::Setup(Display* display)
{
    m_Display = display;
}

void IO::DisplayBuffer::Clear()
{
    m_Cursor = 0;
    m_Buffer.fill(' ');
}

void IO::DisplayBuffer::SetCursor(const int column, const int row)
{
    if (row >= LCD_NUM_LINES || row < 0)
        return;
    if (column >= LCD_LINE_LENGTH || column < 0)
        return;
    m_Cursor = row * LCD_LINE_LENGTH + column;
}

void IO::DisplayBuffer::PrintToDisplay() const
{
    const auto areEqual = std::equal(
        std::begin(m_Buffer), std::end(m_Buffer), std::begin(m_PreviousBuffer));

    if (areEqual)
        return;

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
        m_Display->write(it);
    }
    std::copy(std::begin(m_Buffer), std::end(m_Buffer), std::begin(m_PreviousBuffer));
}
