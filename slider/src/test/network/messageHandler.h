#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <vector>
#include <functional>
#include <utility>

#include "message.h"

namespace Network
{
    class MessageHandler
    {
        class BaseCb
        {   
        public:
            virtual void Invoke(const uint8_t* data, size_t length) const = 0;
            virtual ~BaseCb() {}
        };

        template <class T>
        class Cmd : public BaseCb
        {
            std::function<void(T)>  f_;
            public:
                Cmd(std::function<void(T)> f) : f_(f) {}
                virtual void Invoke(const uint8_t* data, size_t length) const override
                {
                    auto expectedSize = sizeof(MessageWrapper<T>);
                    if (length != expectedSize)
                    {
                        LogDebug("Invalid message size, expected: ", expectedSize, " got: ", length);
                        return;
                    }
                    auto msg = reinterpret_cast<const MessageWrapper<T>*>(data);
                    f_(msg->data);
                }
        };

        public:
            template <class T>
            void AddCb(std::function<void(T)> cb)
            {
                m_Selectors.emplace_back(MessageWrapper<T>::typeId, std::unique_ptr<BaseCb>(new Cmd<T>(cb)));
            }

            template <class T>
            MessageWrapper<T> CreateMessage(const T& message)
            {
                MessageWrapper<T> wrapper;
                wrapper.data = message;
                wrapper.id = MessageWrapper<T>::typeId;
                return wrapper;
            } 

            void Invoke(const uint8_t* data, size_t length)
            {
                auto message = reinterpret_cast<const MessageBase*>(data);
                for(const auto& selector : m_Selectors)
                {
                    if (selector.first == message->id)
                    {
                        auto cmd = selector.second.get();
                        if (cmd)
                            cmd->Invoke(data, length);
                    }
                }
            }

        private:
        using Selector = std::pair<int, std::unique_ptr<BaseCb>>;
        std::vector<Selector> m_Selectors;
    };
}

#endif