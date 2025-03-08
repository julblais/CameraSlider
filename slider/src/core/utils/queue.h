#ifndef CUSTOM_QUEUE_H
#define CUSTOM_QUEUE_H

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

namespace Core
{
    template <typename T>
    struct QueueIterator
    {
    public:
        QueueIterator();
        QueueIterator(QueueHandle_t queue);

        bool operator!=(QueueIterator rhs);
        T& operator*() { return item; }
        void operator++();

    private:
        QueueHandle_t m_Queue;
        T item;
    };

    template <typename T, size_t Length>
    class Queue
    {
    public:
        Queue(const char* name);
        Queue(const Queue&) = delete;
        Queue& operator=(const Queue&) = delete;
        Queue(Queue&&) = default;
        Queue& operator=(Queue&&) = default;

        void push(const T& item);
        template <typename... TArgs> void emplace(TArgs&&... args);
        size_t size();
        QueueIterator<T> begin();
        QueueIterator<T> end();

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
    inline void Queue<T, Length>::push(const T& item)
    {
        xQueueSend(m_Queue, &item, 0);
    }

    template <typename T, size_t Length>
    template<typename... TArgs>
    inline void Queue<T, Length>::emplace(TArgs&& ...args)
    {
        xQueueSend(m_Queue, &T(args...), 0);
    }

    template <typename T, size_t Length>
    inline size_t Queue<T, Length>::size()
    {
        return uxQueueMessagesWaiting(m_Queue);
    }

    template<typename T, size_t Length>
    inline QueueIterator<T> Queue<T, Length>::begin()
    {
        return QueueIterator<T>(m_Queue);
    }

    template<typename T, size_t Length>
    inline QueueIterator<T> Queue<T, Length>::end()
    {
        return QueueIterator<T>();
    }

    template<typename T>
    QueueIterator<T>::QueueIterator()
        : m_Queue(nullptr), item()
    {}

    template<typename T>
    QueueIterator<T>::QueueIterator(QueueHandle_t queue)
        : m_Queue(queue), item()
    {
        this->operator++();
    }

    template<typename T>
    inline bool QueueIterator<T>::operator!=(QueueIterator<T> rhs)
    {
        return m_Queue != rhs.m_Queue;
    }

    template<typename T>
    inline void QueueIterator<T>::operator++()
    {
        ////check queue size
        //xQueueMessagesWaiting(m_Queue);
        if (!xQueueReceive(m_Queue, &item, 0))
            m_Queue = nullptr;
    }
};

#endif