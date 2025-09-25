#ifndef SERIAL_READER_H
#define SERIAL_READER_H

#include "utils/function.h"
#include <vector>

namespace Core
{
    class SerialReader
    {
    public:
        using TMessage = const char*;
        using TListener = function<bool(TMessage message)>;

        SerialReader();
        void ProcessInput() const;
        void AddListener(TListener listener);

        template <typename C>
        auto AddListener(C listener) -> decltype(listener.OnSerialMessage(std::declval<TMessage>()), void())
        {
            m_Listeners.push_back([listener](TMessage message) { return listener.OnSerialMessage(message); });
            return;
        }

        template <typename C>
        auto AddListener(C* listener) -> decltype(listener->OnSerialMessage(std::declval<TMessage>()), void())
        {
            m_Listeners.push_back([listener](TMessage message) { return listener->OnSerialMessage(message); });
            return;
        }

    private:
        std::vector<TListener> m_Listeners;
    };
}

#endif
