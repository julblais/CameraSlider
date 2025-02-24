#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "input.h"
#include "src/core/event/eventSource.h"
#include <functional>
#include "src/core/utils/enumUtils.h"

namespace Input
{
    struct Event
    {
        Event() = default;
        Event(const Event& previous, const InputData& input);
        bool operator==(const Event& other) const;
        bool operator!=(const Event& other) const;

        inline bool HasButtonChange() const
        {
            return button != DpadNone || diff.change != ButtonNone;
        }

        inline bool HasStickMoved() const { return diff.stickMoved; }
        inline bool HasChange() const { return HasButtonChange() || HasStickMoved(); }

        inline ButtonEvent GetButtonEvent() const { return button; }
        inline bool Is(ButtonEvent evt) { return evt == button; }
        inline bool IsDpadUp() const { return button == DpadUp; }
        inline bool IsDpadDown() const { return button == DpadDown; }
        inline bool IsDpadLeft() const { return button == DpadLeft; }
        inline bool IsDpadRight() const { return button == DpadRight; }
        inline bool IsDpadSelect() const { return button == DpadSelect; }
        inline bool IsStickCenter() const { return button == StickCenter; }

        inline float GetStickX() const { return joystickX; }
        inline float GetStickY() const { return joystickY; }

        inline ButtonChange GetButtonChange() const { return diff.change; }
        inline ButtonEvent GetButtonPressed() const { return diff.pressed; }
        inline ButtonEvent GetButtonReleased() const { return diff.released; }

    private:
        struct Diff
        {
            Diff() = default;
            Diff(ButtonEvent pressed, ButtonEvent released, bool stickMoved);
            ButtonEvent released;
            ButtonEvent pressed;
            ButtonChange change;
            bool stickMoved;
        };
        static Diff CreateDiff(const Event& previous, const InputData& input);

        ButtonEvent button;
        float joystickX;
        float joystickY;
        Diff diff;
    };

    class EventDispatcher : public Core::EventSource<const Event&>
    {
    public:
        EventDispatcher() = default;
        void ProcessInput(const InputData& input);
        void Dispatch();
    private:
        InputData m_Input;
        Event m_LastEvent;
        unsigned int m_AggregateCount;
    };
}

#endif