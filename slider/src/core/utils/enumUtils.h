#ifndef ENUMUTILS_H
#define ENUMUTILS_H

namespace Core
{
    template <typename TEnum>
    using EnableIfEnum = std::enable_if<std::is_enum<TEnum>::value, int>::type;

    template<typename T, EnableIfEnum<T> = 0>
    constexpr T operator|(T lhs, T rhs)
    {
        return static_cast<T>(
            static_cast<typename std::underlying_type<T>::type>(lhs) |
            static_cast<typename std::underlying_type<T>::type>(rhs));
    }

    template<typename T, EnableIfEnum<T> = 0>
    constexpr T operator~(T t)
    {
        return static_cast<T>(~static_cast<typename std::underlying_type<T>::type>(t));
    }

    template<typename T, EnableIfEnum<T> = 0>
    constexpr T operator&(T lhs, T rhs)
    {
        return static_cast<T>(
            static_cast<typename std::underlying_type<T>::type>(lhs) &
            static_cast<typename std::underlying_type<T>::type>(rhs));
    }

    template<typename T, EnableIfEnum<T> = 0>
    constexpr T operator^(T lhs, T rhs)
    {
        return static_cast<T>(
            static_cast<typename std::underlying_type<T>::type>(lhs) ^
            static_cast<typename std::underlying_type<T>::type>(rhs));
    }

    template<typename T, EnableIfEnum<T> = 0>
    constexpr T operator|=(T lhs, T rhs)
    {
        return static_cast<T>(
            static_cast<typename std::underlying_type<T>::type>(lhs) |=
            static_cast<typename std::underlying_type<T>::type>(rhs));
    }

    template<typename T, EnableIfEnum<T> = 0>
    constexpr T operator&=(T lhs, T rhs)
    {
        return static_cast<T>(
            static_cast<typename std::underlying_type<T>::type>(lhs) &=
            static_cast<typename std::underlying_type<T>::type>(rhs));
    }

    template<typename T, EnableIfEnum<T> = 0>
    constexpr T operator^=(T lhs, T rhs)
    {
        return static_cast<T>(
            static_cast<typename std::underlying_type<T>::type>(lhs) ^=
            static_cast<typename std::underlying_type<T>::type>(rhs));
    }

    template<typename T, EnableIfEnum<T> = 0>
    constexpr bool HasValue(T current, T value)
    {
        return current & value;
    }
}

#endif