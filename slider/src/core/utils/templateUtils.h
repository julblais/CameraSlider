#ifndef TEMPLATEUTILS_H
#define TEMPLATEUTILS_H

#include <type_traits>

namespace Core
{
    //use this weird thing to avoid undefined order of variadic template arguments
    struct PassParamPack
    {
        template<typename... T>
        PassParamPack(T...) {}
    };

    template<typename, typename = void>
    struct IsTypeComplete : std::false_type {};

    template<typename T>
    struct IsTypeComplete<T, std::void_t<decltype(sizeof(T))> > : std::true_type {};

    template <bool B, class T = void>
    using EnableIf = typename std::enable_if<B, T>::type;

    template <typename TEnum>
    using IsTypeEnum = EnableIf<std::is_enum<TEnum>::value, bool>;

    template <typename T>
    using UnderlyingType = typename std::underlying_type<T>::type;
}

#endif
