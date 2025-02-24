using namespace Core;

template <class TMessage>
inline void MessageHandler::Invoker<TMessage>::Invoke(const uint8_t* data, size_t length) const
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
inline MessageHandler::CallbackHandle MessageHandler::AddCallback(const char* name, std::function<void(T)> callback)
{
    auto ptr = new Invoker<T>(name, callback);
    CallbackHandle handle;
    handle.invoker = ptr;
    m_Selectors.emplace_back(MessageWrapper<T>::typeId, std::unique_ptr<InvokerBase>(ptr));
    return handle;
}

template <class T>
inline MessageWrapper<T> MessageHandler::CreateMessage(const T& message)
{
    return MessageWrapper<T>(MessageWrapper<T>::typeId, message);
}