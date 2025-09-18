#ifndef ENUMUTILS_H
#define ENUMUTILS_H

#include <type_traits>

namespace Core
{
    namespace Enums
    {
        template<bool B, class T = void>
        using enable_if = typename std::enable_if<B, T>::type;

        template<typename TEnum>
        using is_type_enum = enable_if<std::is_enum<TEnum>::value, bool>;

        template<typename T>
        using underlying_type = typename std::underlying_type<T>::type;

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
                static_cast<underlying_type<T>>(lhs) |
                static_cast<underlying_type<T>>(rhs));
        }

        template<typename T, is_type_enum<T> = true>
        constexpr T operator^(T lhs, T rhs)
        {
            return static_cast<T>(
                static_cast<underlying_type<T>>(lhs) ^
                static_cast<underlying_type<T>>(rhs));
        }

        template<typename T, is_type_enum<T> = true>
        constexpr T operator~(T t)
        {
            return static_cast<T>(~static_cast<underlying_type<T>>(t));
        }

        template<typename T, is_type_enum<T> = true>
        constexpr T& operator|=(T& lhs, T rhs)
        {
            return lhs = lhs | rhs;
        }

        template<typename T, is_type_enum<T> = true>
        constexpr T& operator&=(T& lhs, T rhs)
        {
            return lhs = lhs & rhs;
        }

        template<typename T, is_type_enum<T> = true>
        constexpr T& operator^=(T& lhs, T rhs)
        {
            return lhs = lhs ^ rhs;
        }

        template <typename T, is_type_enum<T> = true>
        constexpr bool HasValue(T current, T value)
        {
            return (current & value) == value;
        }

        template<typename T>
        auto GetPreviousValue(T enumValue) -> decltype(T::COUNT)
        {
            const auto val = static_cast<char>(enumValue) - 1;
            const auto count = static_cast<char>(T::COUNT);
            if (val < 0)
                return static_cast<T>(count - 1);
            return static_cast<T>(val);
        }

        template<typename T>
        auto GetNextValue(T enumValue) -> decltype(T::COUNT)
        {
            const auto val = static_cast<char>(enumValue) + 1;
            const auto count = static_cast<char>(T::COUNT);
            if (val >= count)
                return static_cast<T>(0);
            return static_cast<T>(val);
        }
    }
}

#endif
