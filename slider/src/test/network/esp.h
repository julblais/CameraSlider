#ifndef ESP_UTILS_H
#define ESP_UTILS_H

#include <array>
#include <functional>
#include <map>
#include <memory>

#include "src/utils/templateUtils.h"
#include "address.h"
#include "src/debug.h"

template<class T> 
struct MessageTypeId 
{ 
    static_assert(IsTypeComplete<MessageTypeId<T>>::value, "You need to call REGISTER_TYPE_ID.");
};

#define REGISTER_TYPE_ID(T, id_value) \
    template<> struct MessageTypeId<T> \
    { static constexpr unsigned int id = id_value; }; \
    constexpr unsigned int MessageTypeId<T>::id; 

namespace Network
{
    struct MessageBase
    {
        unsigned int id;
    };

    template <typename T>
    struct MessageWrapper : public MessageBase
    {
        T data;
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

    class MessageHandler
    {

        public:
            template <class T>
            void AddCb(std::function<void(T)> cb)
            {
                selector[MessageTypeId<T>::id] = std::unique_ptr<BaseCb>(new Cmd<T>(cb));
            }

            template <class T>
            MessageWrapper<T> CreateMessage(const T& message)
            {
                MessageWrapper<T> wrapper;
                wrapper.data = message;
                wrapper.id = MessageTypeId<T>::id;
                return wrapper;
            } 

            void Invoke(const uint8_t* data, size_t length)
            {
                auto message = reinterpret_cast<const MessageBase*>(data);
                auto it = selector.find(message->id);
                if (it != selector.end())
                {
                    auto cmd = it->second.get();
                    if (cmd)
                        cmd->Invoke(data, length);
                }
            }

        private:
    
        std::map<int, std::unique_ptr<BaseCb>> selector;

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