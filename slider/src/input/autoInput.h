#ifndef AUTOINPUT_H
#define AUTOINPUT_H

#include <initializer_list>
#include <queue>
#include "src/core/time/time.h"
#include "src/core/time/timer.h"
#include "src/input/input.h"

namespace Input
{
    struct InputInstruction
    {
        enum class Command
        {
            Input,
            Hold,
            Pause
        };

        const Core::Time durationMs;
        const InputData input;
        const Command command;

        constexpr InputInstruction(const Core::Time durationMs, const InputData& input)
            :durationMs(durationMs), input(input), command(Command::Hold)
        {}
        constexpr InputInstruction(const InputData& input)
            : durationMs(0), input(input), command(Command::Input)
        {}
        constexpr InputInstruction(const Core::Time durationMs)
            : durationMs(durationMs), input(), command(Command::Pause)
        {}
    };
    
    constexpr InputInstruction DpadUpInstr() { return { InputData(ButtonEvent::Up) }; }
    constexpr InputInstruction DpadDownInstr() { return { InputData(ButtonEvent::Down) }; }
    constexpr InputInstruction DpadLeftInstr() { return { InputData(ButtonEvent::Left) }; }
    constexpr InputInstruction DpadRightInstr() { return { InputData(ButtonEvent::Right) }; }
    constexpr InputInstruction DpadSelectInstr() { return { InputData(ButtonEvent::Select) }; }

    constexpr InputInstruction DpadUpInstr(Core::Time hold) { return { hold, InputData(ButtonEvent::Up) }; }
    constexpr InputInstruction DpadDownInstr(Core::Time hold) { return { hold, InputData(ButtonEvent::Down) }; }
    constexpr InputInstruction DpadLeftInstr(Core::Time hold) { return { hold, InputData(ButtonEvent::Left) }; }
    constexpr InputInstruction DpadRightInstr(Core::Time hold) { return { hold, InputData(ButtonEvent::Right) }; }
    constexpr InputInstruction DpadSelectInstr(Core::Time hold) { return { hold, InputData(ButtonEvent::Select) }; }

    constexpr InputInstruction JoystickInstr(float x, float y) { return { InputData(ButtonEvent::None, x, y) }; }
    constexpr InputInstruction JoystickInstr(float x, float y, Core::Time hold) { return { hold, InputData(ButtonEvent::None, x, y) }; }
    constexpr InputInstruction PauseInstr(Core::Time durationMs) { return { durationMs }; }

    class AutoInput : public Input::InputReader
    {
    public:
        AutoInput(std::initializer_list<InputInstruction> inputs);
        AutoInput(const unsigned int interval, std::initializer_list<InputInstruction> inputs);
        virtual ~AutoInput() = default;
        virtual InputData ReadInput() override;

    private:
        Core::Timer m_Timer;
        std::queue<InputInstruction> m_Instructions;
        bool m_Ready;
    };
}

#endif