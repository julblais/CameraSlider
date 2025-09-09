#ifndef MESSAGECALLBACKHANDLE_H
#define MESSAGECALLBACKHANDLE_H

#include "invokerBase.h"

namespace Core
{
    struct MessageCallbackHandle
    {
        friend class MessageHandler;
    private:
        MessageCallbackHandle(InvokerBase* invoker) : invoker(invoker) {}
        InvokerBase* invoker;
    };
}

#endif