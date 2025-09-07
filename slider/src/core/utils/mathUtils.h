#ifndef MATHUTILS_H
#define MATHUTILS_H

namespace Core
{
    template<typename T>
    T Remap(T value, T low1, T high1, T low2, T high2);

    template<typename T>
    auto GetPreviousEnumValue(T enumValue) -> decltype(T::COUNT);

    template<typename T>
    auto GetNextEnumValue(T enumValue) -> decltype(T::COUNT);
}

#include "mathUtils.tpp"

#endif
