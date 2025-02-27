#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "message.h"
#include "src/debug.h"

#include <vector>
#include <functional>
#include <memory>
#include <utility>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

namespace Core
{
    class InvokerBase
    {
    public:
        InvokerBase(const char* name);
        virtual void Invoke(const uint8_t* data, size_t length) const = 0;
        virtual ~InvokerBase() = default;
        const char* name;
    };

    struct MessageCallbackHandle
    {
        friend class MessageHandler;
    private:
        MessageCallbackHandle(InvokerBase* invoker) : invoker(invoker) {}
        InvokerBase* invoker;
    };

    class MessageHandler
    {
        template <class TMessage>
        class Invoker : public InvokerBase
        {
        public:
            Invoker(const char* name, std::function<void(TMessage)> function);
            virtual void Invoke(const uint8_t* data, size_t length) const override;
        private:
            std::function<void(TMessage)>  m_Function;
        };

        struct QueueItem
        {
            QueueItem();
            QueueItem(const uint8_t* data, size_t length);
            static QueueItem CopyData(const uint8_t* data, size_t length);
            void Finish();

            const uint8_t* data;
            const size_t length;
        };

    public:
        MessageHandler();

        template <class T>
        MessageCallbackHandle AddCallback(const char* name, std::function<void(T)> cb);
        void RemoveCallback(const MessageCallbackHandle& handle);

        template <class T>
        static MessageWrapper<T> CreateMessage(const T& message);

        void Invoke(const uint8_t* data, size_t length) const;
        void ProcessMessages() const;

    private:
        using Selector = std::pair<int, std::unique_ptr<InvokerBase>>;
        std::vector<Selector> m_Selectors;
        QueueHandle_t m_Queue;
    };
}

#include "messageHandler.tpp"

#endif