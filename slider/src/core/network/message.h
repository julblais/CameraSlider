#ifndef MESSAGE_H
#define MESSAGE_H

#include "src/core/utils/templateUtils.h"

#define REGISTER_MESSAGE_TYPE(T, id_value) \
    template<> class Core::MessageWrapper<T> : public MessageBase {\
        public: \
        T data;\
        static constexpr unsigned int ID() { return id_value; } \
        MessageWrapper(T data): MessageBase(id_value), data(data){}\
    };

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
    public:

        static_assert(IsTypeComplete<MessageWrapper<T>>::value, "You need to call REGISTER_TYPE_ID.");
    };
}

#endif