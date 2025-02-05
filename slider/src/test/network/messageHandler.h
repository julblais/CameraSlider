#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "message.h"
#include "src/debug.h"

#include <vector>
#include <functional>
#include <memory>
#include <utility>

namespace Network
{
    class MessageHandler
    {
        class InvokerBase
        {   
            public:
                virtual void Invoke(const uint8_t* data, size_t length) const = 0;
                virtual ~InvokerBase() {}
        };

        template <class TMessage>
        class Invoker : public InvokerBase
        {
            public:
                Invoker(std::function<void(TMessage)> f) : f_(f) {}
                virtual void Invoke(const uint8_t* data, size_t length) const override;
            private:
                std::function<void(TMessage)>  f_;
        };

        public:
            template <class T>
            void AddCallback(std::function<void(T)> cb);

            template <class T>
            MessageWrapper<T> CreateMessage(const T& message);

            void Invoke(const uint8_t* data, size_t length);

        private:
        using Selector = std::pair<int, std::unique_ptr<InvokerBase>>;
        std::vector<Selector> m_Selectors;
    };

    template <class TMessage>
    inline void MessageHandler::Invoker<TMessage>::Invoke(const uint8_t *data, size_t length) const
    {
        auto expectedSize = sizeof(MessageWrapper<TMessage>);
        if (length != expectedSize)
        {
            LogDebug("Invalid message size, expected: ", expectedSize, " got: ", length);
            return;
        }
        auto msg = reinterpret_cast<const MessageWrapper<TMessage>*>(data);
        f_(msg->data);
    }

    template <class T>
    inline void MessageHandler::AddCallback(std::function<void(T)> callback)
    {
        auto ptr = new Invoker<T>(callback);
        m_Selectors.emplace_back(MessageWrapper<T>::typeId, std::unique_ptr<InvokerBase>(ptr));
    }

    template <class T>
    inline MessageWrapper<T> MessageHandler::CreateMessage(const T &message)
    {
        MessageWrapper<T> wrapper;
        wrapper.data = message;
        wrapper.id = MessageWrapper<T>::typeId;
        return wrapper;
    }
}

#endif