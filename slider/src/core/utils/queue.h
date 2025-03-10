#ifndef CUSTOM_QUEUE_H
#define CUSTOM_QUEUE_H

#include <stddef.h>

struct QueueDefinition;
typedef struct QueueDefinition* QueueHandle_t;

namespace Core
{
    namespace QueueUtils
    {
        QueueHandle_t CreateQueue(size_t length, size_t itemSize);
        void Push(const char* name, const void* item, QueueHandle_t queue);
        bool Pop(QueueHandle_t queue, void* const item);
        size_t Size(QueueHandle_t queue);
    };

    template <typename T, size_t Length>
    class Queue
    {
    public:
        Queue(const char* name);
        Queue(Queue&&) = default;
        Queue& operator=(Queue&&) = default;
        Queue(const Queue&) = delete;
        Queue& operator=(const Queue&) = delete;

        void push(const T& item) { QueueUtils::Push(m_Name, &item, m_Queue); }
        template <typename TFunc>
        void foreach(TFunc func) const;
        size_t size() { return QueueUtils::Size(m_Queue); }

    private:
        const char* m_Name;
        QueueHandle_t m_Queue;
    };

    template <typename T, size_t Length>
    Queue<T, Length>::Queue(const char* name)
        : m_Name(name)
    {
        m_Queue = QueueUtils::CreateQueue(Length, sizeof(T));
    }

    template<typename T, size_t Length>
    template<typename TFunc>
    inline void Queue<T, Length>::foreach(TFunc func) const
    {
        T item;
        while (QueueUtils::Pop(m_Queue, &item))
            func(item);
    }
};

#endif