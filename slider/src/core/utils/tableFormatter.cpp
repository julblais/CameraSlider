#include "tableFormatter.h"

using namespace Core;

TableFormatter::TableFormatter(const int cols, const int rows, const int padding, bool hasHeader)
    : m_Cols(cols), m_Rows(hasHeader ? rows + 1 : rows),
    m_HasHeader(hasHeader), m_Padding(padding), m_Data()
{
    m_Data.reserve(cols * rows);
};

void TableFormatter::AddRow(const std::initializer_list<std::string>& values)
{
    for (const auto& value : values)
        m_Data.push_back(value);

}

void TableFormatter::AddRow(const std::vector<std::string>& values)
{
    for (const auto& value : values)
        m_Data.push_back(value);
}

int TableFormatter::CalculateMaxColWith(std::vector<int>& colWidths) const
{
    int sum = 0;
    for (int i = 0; i < m_Cols; i++)
    {
        for (int j = 0; j < m_Rows; j++)
        {
            auto index = j * m_Cols + i;
            auto size = m_Data[index].size() + m_Padding;
            colWidths[i] = std::max((size_t)colWidths[i], size);
        }
    }
    for (int i = 0; i < colWidths.size(); i++)
        sum += colWidths[i];
    return sum;
}

size_t TableFormatter::printTo(Print& p) const
{
    size_t s;
    std::vector<int> colWidths(m_Cols, 0);
    int sum = CalculateMaxColWith(colWidths);

    for (int j = 0; j < m_Rows; j++)
    {
        for (int i = 0; i < m_Cols; i++)
        {
            std::string copy(colWidths[i], ' ');
            auto index = j * m_Cols + i;
            auto str = m_Data[index];
            copy.replace(0, str.size(), str);
            s += p.print(copy.c_str());
            s += p.print("|");
        }
        s += p.println();
        if (j == 0 && m_HasHeader)
        {
            std::string header(sum + m_Cols, '-');
            s += p.println(header.c_str());
        }
    }

    return s;
}
