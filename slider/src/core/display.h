#ifndef DISPLAY_H
#define DISPLAY_H

#include <Print.h>
#include "utils/templateUtils.h"

namespace Core
{
    using Keycode = uint8_t;

    class Display : public Print
    {
    public:
        virtual void SetCursor(const int column, const int row) = 0;
        virtual void Clear() = 0;

        template<typename... TArgs>
        void Print(TArgs&&... args);

        template<typename... TArgs>
        void PrintLine(const int line, TArgs&&... args);

    protected:
        virtual void FillCurrentLine() {}
    };

    template<typename... TArgs>
    void Display::Print(TArgs&&... args)
    {
        PassParamPack { (print(args), 1)... };
    }

    template<typename... TArgs>
    void Display::PrintLine(const int line, TArgs&&... args)
    {
        SetCursor(0, line);
        PassParamPack { (print(args), 1)... };
        FillCurrentLine();
    }
}

#endif
