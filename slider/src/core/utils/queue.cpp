#include "queue.h"
#include "../debug.h"

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#define QUEUE_WARNING_THRESHOLD 3

using namespace Core;

QueueHandle_t QueueUtils::CreateQueue(size_t length, size_t itemSize)
{
    return xQueueCreate(length, itemSize);
}

void QueueUtils::Push(const char* name, const void* item, QueueHandle_t queue)
{
    auto remaining = uxQueueSpacesAvailable(queue);
    if (remaining <= QUEUE_WARNING_THRESHOLD)
        LogWarning("Queue ", name, " has only ", remaining, " spaces left");
    if (xQueueSend(queue, item, 0) != pdPASS)
        LogError("Queue ", name, " has no space left.");
}

bool Core::QueueUtils::Pop(QueueHandle_t queue, void* const item)
{
    return xQueueReceive(queue, item, 0);
}

size_t Core::QueueUtils::Size(QueueHandle_t queue)
{
    return uxQueueMessagesWaiting(queue);
}
