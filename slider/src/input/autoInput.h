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
    struct Instruction
    {
        enum class Type
        {
            Input,
            Hold,
            Pause
        };

        constexpr Instruction(const Core::Time durationMs, const InputData& input)
            :durationMs(durationMs), input(input), type(Type::Hold)
        {}
        constexpr Instruction(const InputData& input)
            : durationMs(0), input(input), type(Type::Input)
        {}
        constexpr Instruction(const Core::Time durationMs)
            : durationMs(durationMs), input(), type(Type::Pause)
        {}

        const Core::Time durationMs;
        const InputData input;
        const Type type;

        static constexpr Instruction Pause(Time durationMs) { return { durationMs }; }
        
        static constexpr Instruction DpadUp() { return { { Input::DpadUp } }; }
        static constexpr Instruction DpadDown() { return { { Input::DpadDown } }; }
        static constexpr Instruction DpadLeft() { return { { Input::DpadLeft } }; }
        static constexpr Instruction DpadRight() { return { { Input::DpadRight } }; }
        static constexpr Instruction DpadSelect() { return { { Input::DpadSelect } }; }

        static constexpr Instruction DpadUp(Time hold) { return { hold, { Input::DpadUp } }; }
        static constexpr Instruction DpadDown(Time hold) { return { hold, { Input::DpadDown } }; }
        static constexpr Instruction DpadLeft(Time hold) { return { hold, { Input::DpadLeft } }; }
        static constexpr Instruction DpadRight(Time hold) { return { hold, { Input::DpadRight } }; }
        static constexpr Instruction DpadSelect(Time hold) { return { hold, { Input::DpadSelect } }; }

        static constexpr Instruction Joystick(float x, float y) { return { { x, y } }; }
        static constexpr Instruction Joystick(float x, float y, Time hold) { return { hold, { x, y } }; }
    };

    class AutoInput : public Input::InputReader
    {
    public:
        AutoInput(std::initializer_list<Instruction> instructions);
        AutoInput(const unsigned int interval, std::initializer_list<Instruction> instructions);
        virtual ~AutoInput() override = default;
        virtual InputData ReadInput() override;

    private:
        Core::Timer m_Timer;
        std::queue<Instruction> m_Instructions;
        bool m_Ready;
    };
}

#endif