#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "message.h"
#include "src/debug.h"
#include "invokerBase.h"
#include "messageCallbackHandle.h"
#include "src/core/utils/queue.h"
#include "src/core/utils/function.h"

#include <vector>
#include <memory>
#include <utility>

#define MESSAGE_QUEUE_LENGTH 20

namespace Core
{
    class MessageHandler
    {
        template <class TMessage>
        class Invoker : public InvokerBase
        {
        public:
            Invoker(const char* name, function<void(TMessage)> function);
            virtual void Invoke(const uint8_t* data, size_t length) const override;
        private:
            function<void(TMessage)>  m_Function;
        };

        struct QueueItem
        {
            QueueItem();
            QueueItem(const uint8_t* data, size_t length);
            void Finish();

            const uint8_t* data;
            const size_t length;
        };

    public:
        MessageHandler();

        template <class T>
        MessageCallbackHandle AddCallback(const char* name, function<void(T)> cb);
        void RemoveCallback(const MessageCallbackHandle& handle);

        template <class T>
        static MessageWrapper<T> CreateMessage(const T& message);

        void Invoke(const uint8_t* data, size_t length);
        void ProcessMessages() const;

    private:
        using Selector = std::pair<int, std::unique_ptr<InvokerBase>>;
        std::vector<Selector> m_Selectors;
        Core::Queue<QueueItem, MESSAGE_QUEUE_LENGTH> m_Queue;
    };
}

#include "messageHandler.tpp"

#endif