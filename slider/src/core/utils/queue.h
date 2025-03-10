#ifndef CUSTOM_QUEUE_H
#define CUSTOM_QUEUE_H

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#define QUEUE_WARNING_THRESHOLD 3

namespace Core
{
    template <typename T, size_t Length>
    class Queue
    {
    public:
        Queue(const char* name);
        Queue(Queue&&) = default;
        Queue& operator=(Queue&&) = default;
        Queue(const Queue&) = delete;
        Queue& operator=(const Queue&) = delete;

        void push(const T& item);
        template <typename TFunc>
        void foreach(TFunc func) const;
        size_t size() { return uxQueueMessagesWaiting(m_Queue); }

    private:
        const char* m_Name;
        QueueHandle_t m_Queue;
    };

    template <typename T, size_t Length>
    Queue<T, Length>::Queue(const char* name)
        : m_Name(name)
    {
        m_Queue = xQueueCreate(Length, sizeof(T));
    }

    template <typename T, size_t Length>
    void Queue<T, Length>::push(const T& item)
    {
        auto remaining = uxQueueSpacesAvailable(m_Queue);
        if (remaining <= QUEUE_WARNING_THRESHOLD)
            LogWarning("Queue ", m_Name, " has only ", remaining, " spaces left");

        if (xQueueSend(m_Queue, &item, 0) != pdPASS)
            LogError("Queue ", m_Name, " has no space left.");
    }

    template<typename T, size_t Length>
    template<typename TFunc>
    inline void Queue<T, Length>::foreach(TFunc func) const
    {
        T item;
        while (xQueueReceive(m_Queue, &item, 0))
            func(item);
    }
};

#endif