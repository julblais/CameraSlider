#ifndef MATHUTILS_H
#define MATHUTILS_H

namespace Core
{
    template <typename T>
    T Remap(T value, T low1, T high1, T low2, T high2)
    {
        return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
    }

    template <typename T>
    auto GetPreviousEnumValue(T enumValue) -> decltype(T::COUNT)
    {
        const auto val = static_cast<char>(enumValue) - 1;
        const auto count = static_cast<char>(T::COUNT);
        if (val < 0)
            return static_cast<T>(count - 1);
        else
            return static_cast<T>(val);
    }

    template <typename T>
    auto GetNextEnumValue(T enumValue) -> decltype(T::COUNT)
    {
        const auto val = static_cast<char>(enumValue) + 1;
        const auto count = static_cast<char>(T::COUNT);
        if (val >= count)
            return static_cast<T>(0);
        else
            return static_cast<T>(val);
    }
}

#endif