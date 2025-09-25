#ifndef DISPLAY_H
#define DISPLAY_H

#include <Print.h>
#include <type_traits>
#include <utility>

namespace Core
{
    using Keycode = uint8_t;
    constexpr char Endl{'\n'};


    //template <typename T>
    //using test = decltype(T::printTo(std::declval<Print&>())->size_t);


    template <bool B, class T = void>
    using enable_if_t = typename std::enable_if<B, T>::type;


    class Display : public Print
    {
    public:
        virtual void SetCursor(const int column, const int row) = 0;
        virtual void Clear() = 0;

        // Primary template: assumes the method does NOT exist
        template <typename, typename = void>
        struct has_PrintToTo : std::false_type {};

        // Specialization: chosen if decltype(...) is valid
        template <typename T>
        struct has_PrintToTo<T,
                             typename std::enable_if<
                                 std::is_same<
                                     decltype(std::declval<T>().printTo(std::declval<::Print&>())),
                                     size_t
                                 >::value
                             >::type
            > : std::true_type {};


        template <typename T,
                  enable_if_t<has_PrintToTo<T>::value, bool> = true>
        size_t print(T&& value)
        {
            return value.printTo(*this);
        }

        template <typename T,
                  enable_if_t<!has_PrintToTo<T>::value, bool> = true>
        size_t print(T&& value)
        {
            return Print::print(std::forward<T>(value));
        }

    protected:
        virtual void FillCurrentLine() {}
    };
}

template <typename T>
Core::Display& operator<<(Core::Display& display, T&& msg)
{
    display.print(msg);
    return display;
}

#endif
