#ifndef MESSAGE_H
#define MESSAGE_H

#include "src/core/utils/templateUtils.h"

#define REGISTER_MESSAGE_TYPE(T, id_value) \
    template<> struct Core::MessageWrapper<T> : public MessageBase \
    { \
        static constexpr unsigned int typeId = id_value; \
        T data; \
        MessageWrapper(int id, T data): MessageBase(id), data(data){}\
    }; \
    constexpr unsigned int Core::MessageWrapper<T>::typeId; 

namespace Core
{
    struct MessageBase
    {
        const int id;
        MessageBase(const int id) : id(id) {}
    };

    template<class T>
    struct MessageWrapper : public MessageBase
    {
        static_assert(IsTypeComplete<MessageWrapper<T>>::value, "You need to call REGISTER_TYPE_ID.");
    };
}

#endif