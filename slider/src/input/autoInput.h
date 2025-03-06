#ifndef AUTOINPUT_H
#define AUTOINPUT_H

#include <initializer_list>
#include <queue>
#include "src/core/time/time.h"
#include "src/core/time/timer.h"
#include "src/input/input.h"

using namespace Core;

namespace Input
{
    struct Command
    {
        enum class Instruction
        {
            Input,
            Hold,
            Pause
        };

        constexpr Command(const Core::Time durationMs, const InputData& input)
            :durationMs(durationMs), input(input), instruction(Instruction::Hold)
        {}
        constexpr Command(const InputData& input)
            : durationMs(0), input(input), instruction(Instruction::Input)
        {}
        constexpr Command(const Core::Time durationMs)
            : durationMs(durationMs), input(), instruction(Instruction::Pause)
        {}

        const Core::Time durationMs;
        const InputData input;
        const Instruction instruction;
    };

    namespace CommandCreator
    {
        constexpr Command DpadUp() { return { { Input::DpadUp } }; }
        constexpr Command DpadDown() { return { { Input::DpadDown } }; }
        constexpr Command DpadLeft() { return { { Input::DpadLeft } }; }
        constexpr Command DpadRight() { return { { Input::DpadRight } }; }
        constexpr Command DpadSelect() { return { { Input::DpadSelect } }; }

        constexpr Command DpadUp(Time hold) { return { hold, { Input::DpadUp } }; }
        constexpr Command DpadDown(Time hold) { return { hold, { Input::DpadDown } }; }
        constexpr Command DpadLeft(Time hold) { return { hold, { Input::DpadLeft } }; }
        constexpr Command DpadRight(Time hold) { return { hold, { Input::DpadRight } }; }
        constexpr Command DpadSelect(Time hold) { return { hold, { Input::DpadSelect } }; }

        constexpr Command Joystick(float x, float y) { return { { x, y } }; }
        constexpr Command Joystick(float x, float y, Time hold) { return { hold, { x, y } }; }
        constexpr Command Pause(Time durationMs) { return { durationMs }; }
    }

    class AutoInput : public Input::InputReader
    {
    public:
        AutoInput(std::initializer_list<Command> commands);
        AutoInput(const unsigned int interval, std::initializer_list<Command> commands);
        virtual ~AutoInput() = default;
        virtual InputData ReadInput() override;

    private:
        Core::Timer m_Timer;
        std::queue<Command> m_Commands;
        bool m_Ready;
    };
}

#endif