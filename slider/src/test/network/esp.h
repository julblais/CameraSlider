#ifndef ESP_UTILS_H
#define ESP_UTILS_H

#include <vector>
#include <functional>
#include <utility>
#include <memory>

#include "src/utils/templateUtils.h"
#include "address.h"
#include "src/debug.h"

#define REGISTER_MESSAGE_TYPE(T, id_value) \
    template<> struct Network::MessageHandler::MessageWrapper<T> : public MessageBase \
    { \
        static constexpr unsigned int typeId = id_value; \
        T data; \
    }; \
    constexpr unsigned int Network::MessageHandler::MessageWrapper<T>::typeId; 

namespace Network
{
    class MessageHandler
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

    class Esp
    {
        public:
            using ReceiveCallback = std::function<void(const MacAddress&, const uint8_t*, int)>;
            using SendCallback = std::function<void(const MacAddress&, bool)>;

            static bool Init();
            static MacAddress GetMacAddress();
            static bool AddPeer(const MacAddress& address);
            static bool RemovePeer(const MacAddress& address);
            static void RegisterReceiveCallback(ReceiveCallback callback);
            static void RegisterSendCallback(SendCallback callback);

            static void Send(const uint8_t *data, size_t len);
            static void Send(const MacAddress& address, const uint8_t* data, size_t len);
            
            template <typename Message>
            static void Send(const MacAddress& address, const Message& message);

            template <typename Message>
            static void Send(const Message& message);
    };

    template <typename Message>
    inline void Esp::Send(const MacAddress &address, const Message &message)
    {
        Send(address, reinterpret_cast<const uint8_t*>(&message), sizeof(Message));
    }

    template <typename Message>
    inline void Esp::Send(const Message &message)
    {
        Send(reinterpret_cast<const uint8_t*>(&message), sizeof(Message));
    }
}

#endif