#include "messages.h"

using namespace Messages;

BrainMessage BrainMessage::BuildDisplayTextMessage(const std::array<char, TEXT_SIZE> &text)
{
    BrainMessage message
    {
        .type = Type::DisplayText,
        .text = text 
    };
    return message;
}

ControllerMessage ControllerMessage::BuildDisplayTextMessage(const Input::Event& event)
{
    ControllerMessage message
    {
        .type = Type::InputEvent,
        .event = event
    };
    return message;
}
