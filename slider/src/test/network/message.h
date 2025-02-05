#ifndef MESSAGE_H
#define MESSAGE_H

#include "src/utils/templateUtils.h"

#define REGISTER_MESSAGE_TYPE(T, id_value) \
    template<> struct Network::MessageWrapper<T> : public MessageBase \
    { \
        static constexpr unsigned int typeId = id_value; \
        T data; \
    }; \
    constexpr unsigned int Network::MessageWrapper<T>::typeId; 

namespace Network
{
    struct MessageBase
    {
        unsigned int id;
    };

    template<class T> 
    struct MessageWrapper : public MessageBase
    { 
        static_assert(IsTypeComplete<MessageWrapper<T>>::value, "You need to call REGISTER_TYPE_ID.");
    };
}

#endif