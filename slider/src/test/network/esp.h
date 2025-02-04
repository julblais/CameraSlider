#ifndef ESP_UTILS_H
#define ESP_UTILS_H

#include <array>
#include <functional>
#include <map>
#include <memory>

#include "src/utils/templateUtils.h"
#include "address.h"

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
        //virtual ~MessageBase() = default;
        unsigned int id;
    };


    struct MessageExample : public MessageBase
    {
        int x;
        int y;
    };
    struct CustomMessage : public MessageBase
    {
        int x;
        int y;
        bool toto;
    };

    template<class T>
    using MessageCb = std::function<void(T)>; 

    class BaseCb
    {   
    public:
        virtual void Invoke(const MessageBase* arg) const = 0;
        virtual ~BaseCb() {}
    };

    template <class ArgType>
    class Cmd : public BaseCb
    {
        typedef std::function<void(ArgType)> FuncType;
        FuncType f_;
        public:
            Cmd(FuncType f) : f_(f) {}
            virtual void Invoke(const MessageBase* arg) const override
            {
                auto msg = reinterpret_cast<const ArgType*>(arg);
                f_(*msg);
            }
    };

    class MessageHandler
    {

        public:
            template <class T>
            void AddCb(MessageCb<T> cb)
            {
                selector[MessageTypeId<T>::id] = std::unique_ptr<BaseCb>(new Cmd<T>(cb));
            }

            void Invoke(const uint8_t* data)
            {
                MessageBase myData;
                //memcpy(&myData, incomingData, sizeof(myData));
                auto message = reinterpret_cast<const MessageBase*>(data);
                auto it = selector.find(message->id);
                if (it != selector.end())
                {
                    auto cmd = it->second.get();
                    if (cmd)
                        cmd->Invoke(message);
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