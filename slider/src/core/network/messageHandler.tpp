#include "messageHandler.h"
using namespace Core;

template <class TMessage>
MessageHandler::Invoker<TMessage>::Invoker(const char* name, std::function<void(TMessage)> function)
    : InvokerBase(name), m_Function(function)
{}

template <class TMessage>
void MessageHandler::Invoker<TMessage>::Invoke(const uint8_t* data, size_t length) const
{
    auto expectedSize = sizeof(MessageWrapper<TMessage>);
    if (length != expectedSize)
    {
        LogDebug("Invalid message size, expected: ", expectedSize, " got: ", length);
        return;
    }
    auto msg = reinterpret_cast<const MessageWrapper<TMessage>*>(data);
    m_Function(msg->data);
}

template <class T>
MessageCallbackHandle MessageHandler::AddCallback(const char* name, std::function<void(T)> callback)
{
    auto ptr = new Invoker<T>(name, callback);
    m_Selectors.emplace_back(MessageWrapper<T>::StaticId(), std::unique_ptr<InvokerBase>(ptr));
    return MessageCallbackHandle(ptr);
}

template <class T>
MessageWrapper<T> MessageHandler::CreateMessage(const T& message)
{
    return MessageWrapper<T>(message);
}
