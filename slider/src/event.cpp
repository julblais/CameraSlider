#include "event.h"
#include "core/debug.h"
#include <memory>

using namespace IO;

std::unique_ptr<char[]> IO::ToString(const ButtonEvent button)
{
    std::unique_ptr<char[]> buffer(new char[BUTTON_EVENT_COUNT + 1]);
    int index = 0;
    if (button.Has(ButtonDpadUp))
        buffer[index++] = 'U';
    if (button.Has(ButtonDpadDown))
        buffer[index++] = 'D';
    if (button.Has(ButtonDpadLeft))
        buffer[index++] = 'L';
    if (button.Has(ButtonDpadRight))
        buffer[index++] = 'R';
    if (button.Has(ButtonSelect))
        buffer[index++] = 'S';
    if (button.Has(ButtonCenter))
        buffer[index++] = 'C';

    buffer[index] = '\0';
    return buffer;
}

Event::Event() :
    m_Previous(ButtonNone),
    m_Current(ButtonNone) {}

Event::Event(const InputData& previous, const InputData& input) :
    m_Previous(previous),
    m_Current(input) {}

Event::Event(const Event& previous, const InputData& input) :
    Event(previous.m_Current, input) {}

bool Event::HasButtonChange() const
{
    return (m_Previous.buttons | m_Current.buttons).Any();
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
    return GetReleasedButtons().Any();
}

bool Event::IsDpadUp() const
{
    return GetReleasedButtons().Has(ButtonDpadUp);
}

bool Event::IsDpadDown() const
{
    return GetReleasedButtons().Has(ButtonDpadDown);
}

bool Event::IsDpadLeft() const
{
    return GetReleasedButtons().Has(ButtonDpadLeft);
}

bool Event::IsDpadRight() const
{
    return GetReleasedButtons().Has(ButtonDpadRight);
}

bool Event::IsDpadSelect() const
{
    return GetReleasedButtons().Has(ButtonSelect);
}

bool Event::IsStickCenter() const
{
    return GetReleasedButtons().Has(ButtonCenter);
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
