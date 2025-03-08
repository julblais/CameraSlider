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
        struct Iterator
        {
            friend class Queue;
        public:
            const T& operator*() const { return item; }
            bool operator!=(Iterator rhs) { return m_Queue != rhs.m_Queue; }
            void operator++();
        private:
            Iterator() : m_Queue(nullptr), item() {}
            Iterator(QueueHandle_t queue) : m_Queue(queue), item() { this->operator++(); }

            QueueHandle_t m_Queue;
            T item;
        };

        Queue(const char* name);
        Queue(Queue&&) = default;
        Queue& operator=(Queue&&) = default;
        Queue(const Queue&) = delete;
        Queue& operator=(const Queue&) = delete;

        void push(const T& item);
        size_t size() { return uxQueueMessagesWaiting(m_Queue); }

        Iterator begin() const { return Iterator(m_Queue); }
        Iterator end() const { return Iterator(); }

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
    inline void Queue<T, Length>::Iterator::operator++()
    {
        if (!xQueueReceive(m_Queue, &item, 0))
            m_Queue = nullptr;
    }
};

#endif