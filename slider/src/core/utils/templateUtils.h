#ifndef TEMPLATEUTILS_H
#define TEMPLATEUTILS_H

#include <type_traits>

namespace Core
{
    //use this weird thing to avoid undefined order of variadic template arguments
    struct PassParamPack { template<typename ...T> PassParamPack(T...) {} };

    //check if is type is complete (specialized)
    namespace {
        template <class T, std::size_t = sizeof(T)>
        std::true_type IsTypeCompleteImpl(T*);
        std::false_type IsTypeCompleteImpl(...);
    }
    template <class T>
    using IsTypeComplete = decltype(IsTypeCompleteImpl(std::declval<T*>()));
}

#endif