#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "core/eventSource.h"
#include "memory"
#include "Print.h"
#include "core/utils/enumFlagsWrapper.h"

#define BUTTON_EVENT_COUNT 6

namespace IO
{
    enum class ButtonEventFlags
    {
        None = 0,
        Up = 1 << 0,
        Down = 1 << 1,
        Left = 1 << 2,
        Right = 1 << 3,
        Select = 1 << 4,
        Center = 1 << 5
    };

    using ButtonEvent = Core::EnumFlagsWrapper<ButtonEventFlags>;

    constexpr auto ButtonNone = ButtonEvent(ButtonEventFlags::None);
    constexpr auto ButtonDpadUp = ButtonEvent(ButtonEventFlags::Up);
    constexpr auto ButtonDpadDown = ButtonEvent(ButtonEventFlags::Down);
    constexpr auto ButtonDpadLeft = ButtonEvent(ButtonEventFlags::Left);
    constexpr auto ButtonDpadRight = ButtonEvent(ButtonEventFlags::Right);
    constexpr auto ButtonCenter = ButtonEvent(ButtonEventFlags::Center);
    constexpr auto ButtonSelect = ButtonEvent(ButtonEventFlags::Select);

    std::unique_ptr<char[]> ToString(const ButtonEvent button);

    struct InputData
    {
        constexpr InputData() :
            InputData(ButtonNone, 0.0f, 0.0f) {}

        constexpr InputData(const ButtonEvent buttons) :
            InputData(buttons, 0.0f, 0.0f) {}

        constexpr InputData(const float x, const float y) :
            InputData(ButtonNone, x, y) {}

        constexpr InputData(const ButtonEvent buttons, const float x, const float y) :
            buttons(buttons),
            x(x),
            y(y) {}

        ButtonEvent buttons;
        float x;
        float y;
    };

    struct Event : Printable
    {
        Event();
        Event(const Event& previous, const InputData& input);
        Event(const InputData& previous, const InputData& input);

        bool HasButtonChange() const;
        bool HasStickMoved() const;
        bool HasChange() const;

        ButtonEvent GetPressedButtons() const;
        ButtonEvent GetReleasedButtons() const;
        float GetStickX() const;
        float GetStickY() const;

        bool HasActiveButton() const;
        bool IsDpadUp() const;
        bool IsDpadDown() const;
        bool IsDpadLeft() const;
        bool IsDpadRight() const;
        bool IsDpadSelect() const;
        bool IsStickCenter() const;

        void Log() const;

    protected:
        size_t printTo(Print& p) const override;

    private:
        const InputData m_Previous;
        const InputData m_Current;
    };
}

#endif
