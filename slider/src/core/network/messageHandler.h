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
    class MessageHandler
    {
    private:
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
            Invoker(std::function<void(TMessage)> function) : m_Function(function) {}
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
        void AddCallback(std::function<void(T)> cb);

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