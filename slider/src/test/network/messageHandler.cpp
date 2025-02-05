#include "messageHandler.h"

void Network::MessageHandler::Invoke(const uint8_t *data, size_t length)
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
