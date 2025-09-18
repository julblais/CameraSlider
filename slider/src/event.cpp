#include "event.h"
#include "core/debug.h"
#include "core/utils/enumUtils.h"
#include <memory>

using namespace IO;
using namespace Core::Enums;

std::unique_ptr<char[]> IO::ToString(const ButtonEvent button)
{
    std::unique_ptr<char[]> buffer(new char[BUTTON_EVENT_COUNT + 1]);
    int index = 0;
    if (HasValue(button, ButtonEvent::Up))
        buffer[index++] = 'U';
    if (HasValue(button, ButtonEvent::Down))
        buffer[index++] = 'D';
    if (HasValue(button, ButtonEvent::Left))
        buffer[index++] = 'L';
    if (HasValue(button, ButtonEvent::Right))
        buffer[index++] = 'R';
    if (HasValue(button, ButtonEvent::Select))
        buffer[index++] = 'S';
    if (HasValue(button, ButtonEvent::Center))
        buffer[index++] = 'C';

    buffer[index] = '\0';
    return buffer;
}

Event::Event() :
    m_Previous(ButtonEvent::None),
    m_Current(ButtonEvent::None) {}

Event::Event(const InputData& previous, const InputData& input) :
    m_Previous(previous),
    m_Current(input) {}

Event::Event(const Event& previous, const InputData& input) :
    Event(previous.m_Current, input) {}

bool Event::HasButtonChange() const
{
    return (m_Previous.buttons | m_Current.buttons) != ButtonEvent::None;
}

bool Event::HasStickMoved() const
{
    return m_Previous.x != m_Current.x || m_Previous.y != m_Current.y;
}

bool Event::HasChange() const
{
    return HasButtonChange() || HasStickMoved();
}

ButtonEvent Event::GetPressedButtons() const
{
    return m_Current.buttons;
}

ButtonEvent Event::GetReleasedButtons() const
{
    return m_Previous.buttons & ~m_Current.buttons;
}

float Event::GetStickX() const
{
    return m_Current.x;
}

float Event::GetStickY() const
{
    return m_Current.y;
}

bool Event::HasActiveButton() const
{
    return GetReleasedButtons() != ButtonEvent::None;
}

bool Event::IsDpadUp() const
{
    return HasValue(GetReleasedButtons(), ButtonEvent::Up);
}

bool Event::IsDpadDown() const
{
    return HasValue(GetReleasedButtons(), ButtonEvent::Down);
}

bool Event::IsDpadLeft() const
{
    return HasValue(GetReleasedButtons(), ButtonEvent::Left);
}

bool Event::IsDpadRight() const
{
    return HasValue(GetReleasedButtons(), ButtonEvent::Right);
}

bool Event::IsDpadSelect() const
{
    return HasValue(GetReleasedButtons(), ButtonEvent::Select);
}

bool Event::IsStickCenter() const
{
    return HasValue(GetReleasedButtons(), ButtonEvent::Center);
}

void Event::Log() const
{
    if (HasButtonChange())
    {
        LogInfo("Event buttons:\t", ToString(GetPressedButtons()).get());
    }
    if (HasStickMoved())
    {
        LogInfo("Event joystick\t", GetStickX(), "\t", GetStickY());
    }
}

size_t Event::printTo(Print& p) const
{
    size_t n = 0;
    const auto pressed = ToString(GetPressedButtons());
    n += p.print("Btns:");
    n += p.println(pressed.get());
    n += p.print("(");
    n += p.print(GetStickX());
    n += p.print(",");
    n += p.print(GetStickY());
    n += p.print(")");
    return n;
}
