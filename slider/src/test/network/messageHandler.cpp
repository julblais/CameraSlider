#include "messageHandler.h"

#define QUEUE_LENGTH 10

Network::QueueItem::QueueItem()
    :data(0), length(0) {}

Network::QueueItem::QueueItem(const uint8_t *data, size_t length)
    : data(data), length(length) {}

Network::QueueItem Network::QueueItem::CopyData(const uint8_t *data, size_t length)
{
    auto dataCopy = new uint8_t[length];
    std::copy(data, data + length, dataCopy);
    return QueueItem(dataCopy, length);
}

void Network::QueueItem::Finish()
{
    delete[] data;
}

Network::MessageHandler::MessageHandler()
{
    m_Queue = xQueueCreate(QUEUE_LENGTH, sizeof(QueueItem));
}

void Network::MessageHandler::ProcessMessages() const
{
    QueueItem item;
    if (xQueueReceive(m_Queue, &item, 0))
    {
        auto message = reinterpret_cast<const MessageBase*>(item.data);
        for(const auto& selector : m_Selectors)
        {
            if (selector.first == message->id)
            {
                auto cmd = selector.second.get();
                if (cmd)
                    cmd->Invoke(item.data, item.length);
            }
        }
        item.Finish();
    }
}

void Network::MessageHandler::Invoke(const uint8_t *data, size_t length) const
{
    //copy data in dynamic array
    QueueItem item = QueueItem::CopyData(data, length);
    xQueueSend(m_Queue, &item, 0);
}