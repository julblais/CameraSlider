#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "input.h"
#include "core/eventSource.h"

namespace Input
{
    struct Event
    {
        Event();
        Event(const Event& previous, const InputData& input);

        bool operator==(const Event& rhs) const;
        bool operator!=(const Event& rhs) const;

        bool HasButtonChange() const
        {
            return button != None || diff.change != ButtonNone;
        }

        bool HasStickMoved() const { return diff.stickMoved; }
        bool HasChange() const { return HasButtonChange() || HasStickMoved(); }

        ButtonEvent GetButtonEvent() const { return button; }
        bool Is(const ButtonEvent evt) const { return evt == button; }
        bool IsDpadUp() const { return button == DpadUp; }
        bool IsDpadDown() const { return button == DpadDown; }
        bool IsDpadLeft() const { return button == DpadLeft; }
        bool IsDpadRight() const { return button == DpadRight; }
        bool IsDpadSelect() const { return button == DpadSelect; }
        bool IsStickCenter() const { return button == StickCenter; }

        float GetStickX() const { return joystickX; }
        float GetStickY() const { return joystickY; }

        ButtonChange GetButtonChange() const { return diff.change; }
        ButtonEvent GetButtonPressed() const { return diff.pressed; }
        ButtonEvent GetButtonReleased() const { return diff.released; }

    private:
        struct Diff
        {
            Diff();
            Diff(const ButtonEvent pressed, const ButtonEvent released, const bool stickMoved);
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

    class EventDispatcher : public EventSource<const Event&>
    {
    public:
        EventDispatcher();
        void ProcessInput(const InputData& input);
        void Dispatch();

    private:
        InputData m_Input;
        Event m_LastEvent;
        unsigned int m_AggregateCount;
    };
}

#endif
