#include "messageHandler.h"
#include "src/core/utils/vectorUtils.h"

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
    if (EraseFirstIf(m_Selectors, [handle](const Selector& sel) {
        return sel.second.get() == handle.invoker; }))
        return;
    LogError("Cannot remove callback, not found: ", handle.invoker->name);
}

void MessageHandler::ProcessMessages() const
{
    m_Queue.foreach([this](MessageHandler::QueueItem& item) {
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
    });
}

void MessageHandler::Invoke(const uint8_t* data, size_t length)
{
    auto dataCopy = new uint8_t[length];
    std::copy(data, data + length, dataCopy);
    m_Queue.push(QueueItem(dataCopy, length));
}