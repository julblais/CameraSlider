#ifndef DISPLAY_H
#define DISPLAY_H

#include <Print.h>
#include "utils/templateUtils.h"
#include <type_traits>
#include <utility>

namespace Core
{
    using Keycode = uint8_t;
    constexpr char Endl{'\n'};

    class Display : public Print
    {
        template <typename T>
        using PrintToExists = decltype(std::declval<T>().printTo(std::declval<::Print&>()));

        template <typename T>
        using PrintToEnableIf = std::enable_if<std::is_same<PrintToExists<T>, size_t>::value>;

        template <typename, typename = void>
        struct HasPrintTo : std::false_type {};

        template <typename T>
        struct HasPrintTo<T, typename PrintToEnableIf<T>::type> : std::true_type {};

    public:
        virtual void SetCursor(const int column, const int row) = 0;
        virtual void Clear() = 0;

        template <typename T, enable_if_t<HasPrintTo<T>::value, bool> = true>
        size_t print(T&& value);

        template <typename T, enable_if_t<!HasPrintTo<T>::value, bool> = true>
        size_t print(T&& value);

    protected:
        virtual void FillCurrentLine() {}
    };

    template <typename T, enable_if_t<Display::HasPrintTo<T>::value, bool>>
    size_t Display::print(T&& value)
    {
        return value.printTo(*this);
    }

    template <typename T, enable_if_t<!Display::HasPrintTo<T>::value, bool>>
    size_t Display::print(T&& value)
    {
        return Print::print(std::forward<T>(value));
    }
}

template <typename T>
Core::Display& operator<<(Core::Display& display, T&& msg)
{
    display.print(msg);
    return display;
}

#endif
