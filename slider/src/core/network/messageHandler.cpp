#include "messageHandler.h"

#define QUEUE_LENGTH 10

using namespace Core;

MessageHandler::QueueItem::QueueItem()
    :data(0), length(0)
{}

MessageHandler::QueueItem::QueueItem(const uint8_t* data, size_t length)
    :data(data), length(length)
{}

void MessageHandler::QueueItem::Finish()
{
    delete[] data;
}

MessageHandler::MessageHandler()
    :m_Queue("Message queue")
{}

void MessageHandler::RemoveCallback(const MessageCallbackHandle& handle)
{
    auto it = m_Selectors.begin();
    while (it != m_Selectors.end())
    {
        if (it->second.get() == handle.invoker)
        {
            m_Selectors.erase(it);
            return;
        }
    }
    LogError("Cannot remove callback, not found: ", handle.invoker->name);
}

void MessageHandler::ProcessMessages() const
{
    for (auto item : m_Queue)
    {
        auto message = reinterpret_cast<const MessageBase*>(item.data);
        for (const auto& selector : m_Selectors)
        {
            if (selector.first == message->id)
            {
                auto cmd = selector.second.get();
                if (cmd)
                {
                    LogDebug("Invoking: ", cmd->name);
                    cmd->Invoke(item.data, item.length);
                }
            }
        }
        item.Finish();
    }
}

void MessageHandler::Invoke(const uint8_t* data, size_t length)
{
    auto dataCopy = new uint8_t[length];
    std::copy(data, data + length, dataCopy);
    m_Queue.push(QueueItem(dataCopy, length));
}