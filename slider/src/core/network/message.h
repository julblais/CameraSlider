#ifndef MESSAGE_H
#define MESSAGE_H

#include "src/core/utils/templateUtils.h"

#define REGISTER_MESSAGE_TYPE(T, id_value) \
    template<> class Core::MessageWrapper<T> : public MessageBase {\
        public: \
            static constexpr unsigned int StaticId() { return id_value; } \
            T data;\
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