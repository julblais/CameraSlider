#ifndef ENUMUTILS_H
#define ENUMUTILS_H

#include <type_traits>

namespace Core
{
    namespace Enums
    {
        template< bool B, class T = void >
        using enable_if = typename std::enable_if<B, T>::type;

        template<typename TEnum>
        using is_type_enum = enable_if<std::is_enum<TEnum>::value, bool>;

        template<typename T>
        using underlying_type = typename std::underlying_type<T>::type;

        template<typename T, is_type_enum<T> = true>
        constexpr bool HasValue(T current, T value)
        {
            return (current & value) == value;
        }

        template<typename T, is_type_enum<T> = true>
        constexpr T operator&(T lhs, T rhs)
        {
            return static_cast<T>(
                static_cast<underlying_type<T>>(lhs) &
                static_cast<underlying_type<T>>(rhs));
        }

        template<typename T, is_type_enum<T> = true>
        constexpr T operator|(T lhs, T rhs)
        {
            return static_cast<T>(
                static_cast<underlying_type<T>>(lhs) &
                static_cast<underlying_type<T>>(rhs));
        }

        template<typename T, is_type_enum<T> = true>
        constexpr T operator~(T t)
        {
            return static_cast<T>(~static_cast<underlying_type<T>>(t));
        }

        template<typename T, is_type_enum<T> = true>
        constexpr T operator^(T lhs, T rhs)
        {
            return static_cast<T>(
                static_cast<underlying_type<T>>(lhs) ^
                static_cast<underlying_type<T>>(rhs));
        }

        template<typename T, is_type_enum<T> = true>
        constexpr T operator|=(T lhs, T rhs)
        {
            return static_cast<T>(
                static_cast<underlying_type<T>>(lhs) |=
                static_cast<underlying_type<T>>(rhs));
        }

        template<typename T, is_type_enum<T> = true>
        constexpr T operator&=(T lhs, T rhs)
        {
            return static_cast<T>(
                static_cast<underlying_type<T>>(lhs) &=
                static_cast<underlying_type<T>>(rhs));
        }

        template<typename T, is_type_enum<T> = true>
        constexpr T operator^=(T lhs, T rhs)
        {
            return static_cast<T>(
                static_cast<underlying_type<T>>(lhs) ^=
                static_cast<underlying_type<T>>(rhs));
        }
    }
}

#endif