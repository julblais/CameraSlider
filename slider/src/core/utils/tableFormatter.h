#ifndef TABLEFORMATTER_H
#define TABLEFORMATTER_H

#include <vector>
#include <string>
#include <Print.h>

namespace Core
{
    class TableFormatter : public Printable
    {
    public:
        TableFormatter(const int cols, const int rows, const int padding = 0, bool hasHeader = true);

        void AddRow(const std::initializer_list<std::string>& values);
        void AddRow(const std::vector<std::string>& values);
        virtual size_t printTo(Print& p) const;
        
    private:
        int CalculateMaxColWith(std::vector<int>& colWidths) const;

        const int m_Cols;
        const int m_Rows;
        const int m_Padding;
        bool m_HasHeader;
        std::vector<std::string> m_Data;
    };
}

#endif