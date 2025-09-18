#ifndef ENUMUTILS_H
#define ENUMUTILS_H

#include "templateUtils.h"

namespace Core
{
    template <typename T, IsTypeEnum<T> = true>
    constexpr T operator&(T lhs, T rhs)
    {
        return static_cast<T>(
            static_cast<UnderlyingType<T>>(lhs) &
            static_cast<UnderlyingType<T>>(rhs));
    }

    template <typename T, IsTypeEnum<T> = true>
    constexpr T operator|(T lhs, T rhs)
    {
        return static_cast<T>(
            static_cast<UnderlyingType<T>>(lhs) |
            static_cast<UnderlyingType<T>>(rhs));
    }

    template <typename T, IsTypeEnum<T> = true>
    constexpr T operator^(T lhs, T rhs)
    {
        return static_cast<T>(
            static_cast<UnderlyingType<T>>(lhs) ^
            static_cast<UnderlyingType<T>>(rhs));
    }

    template <typename T, IsTypeEnum<T> = true>
    constexpr T operator~(T t)
    {
        return static_cast<T>(~static_cast<UnderlyingType<T>>(t));
    }

    template <typename T, IsTypeEnum<T> = true>
    constexpr T& operator|=(T& lhs, T rhs)
    {
        return lhs = lhs | rhs;
    }

    template <typename T, IsTypeEnum<T> = true>
    constexpr T& operator&=(T& lhs, T rhs)
    {
        return lhs = lhs & rhs;
    }

    template <typename T, IsTypeEnum<T> = true>
    constexpr T& operator^=(T& lhs, T rhs)
    {
        return lhs = lhs ^ rhs;
    }

    template <typename T, IsTypeEnum<T> = true>
    constexpr bool HasValue(T current, T value)
    {
        return (current & value) == value;
    }

    template <typename T>
    auto GetPreviousEnumValue(T enumValue) -> decltype(T::COUNT)
    {
        const auto val = static_cast<char>(enumValue) - 1;
        const auto count = static_cast<char>(T::COUNT);
        if (val < 0)
            return static_cast<T>(count - 1);
        return static_cast<T>(val);
    }

    template <typename T>
    auto GetNextEnumValue(T enumValue) -> decltype(T::COUNT)
    {
        const auto val = static_cast<char>(enumValue) + 1;
        const auto count = static_cast<char>(T::COUNT);
        if (val >= count)
            return static_cast<T>(0);
        return static_cast<T>(val);
    }
}

#endif
