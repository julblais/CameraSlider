#ifndef DISPLAY_H
#define DISPLAY_H

#include <Print.h>
#include "src/core/utils/templateUtils.h"

namespace Core
{
    using Keycode = uint8_t;

    enum class Symbol : char
    {
        LeftRightArrows,
        UpDownArrows
    };

    class SymbolHandle : public Printable
    {
    public:
        SymbolHandle(Keycode id) : m_Id(id) {};

        virtual size_t printTo(Print& p) const override
        {
            return p.write(m_Id);
        }

    private:
        uint8_t m_Id;
    };

    class Display : public Print
    {
    public:
        virtual void Init() {}
        virtual void SetCursor(const int column, const int row) = 0;
        virtual SymbolHandle GetSymbol(Symbol symbol) const = 0;
        virtual void Clear() = 0;

        template <typename... TArgs> void Print(TArgs&&... args);
        template <typename... TArgs> void PrintLine(const int line, TArgs&&... args);
    protected:
        virtual void FillCurrentLine() {}
    };

    template <typename... TArgs>
    inline void Display::Print(TArgs &&...args)
    {
        Core::PassParamPack { (print(args), 1)... };
    }

    template <typename... TArgs>
    void Display::PrintLine(const int line, TArgs &&...args)
    {
        SetCursor(0, line);
        Core::PassParamPack { (print(args), 1)... };
        FillCurrentLine();
    }
}

#endif