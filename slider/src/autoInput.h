#ifndef AUTOINPUT_H
#define AUTOINPUT_H

#include <initializer_list>
#include <queue>
#include "core/time/time.h"
#include "core/timer.h"
#include "input.h"

using namespace Core;

namespace IO
{
    struct Instruction
    {
        enum class Type
        {
            Input,
            Hold,
            Pause
        };

        constexpr Instruction(const Time durationMs, const InputData& input) :
            durationMs(durationMs),
            input(input),
            type(Type::Hold)
        {}

        constexpr Instruction(const InputData& input) :
            durationMs(0),
            input(input),
            type(Type::Input)
        {}

        constexpr Instruction(const Time durationMs) :
            durationMs(durationMs),
            input(),
            type(Type::Pause)
        {}

        const Time durationMs;
        const InputData input;
        const Type type;

        static constexpr Instruction Pause(Time durationMs) { return { durationMs }; }

        static constexpr Instruction DpadUp() { return {{IO::DpadUp}}; }
        static constexpr Instruction DpadDown() { return {{IO::DpadDown}}; }
        static constexpr Instruction DpadLeft() { return {{IO::DpadLeft}}; }
        static constexpr Instruction DpadRight() { return {{IO::DpadRight}}; }
        static constexpr Instruction DpadSelect() { return {{IO::DpadSelect}}; }

        static constexpr Instruction DpadUp(Time hold) { return {hold, {IO::DpadUp}}; }
        static constexpr Instruction DpadDown(Time hold) { return {hold, {IO::DpadDown}}; }
        static constexpr Instruction DpadLeft(Time hold) { return {hold, {IO::DpadLeft}}; }
        static constexpr Instruction DpadRight(Time hold) { return {hold, {IO::DpadRight}}; }
        static constexpr Instruction DpadSelect(Time hold) { return {hold, {IO::DpadSelect}}; }

        static constexpr Instruction Joystick(float x, float y) { return { { x, y } }; }
        static constexpr Instruction Joystick(float x, float y, Time hold) { return { hold, { x, y } }; }
    };

    class AutoInput
    {
    public:
        AutoInput(const std::initializer_list<Instruction> instructions);
        AutoInput(const unsigned int interval, const std::initializer_list<Instruction> instructions);
        ~AutoInput() = default;
        InputData ReadInput();

    private:
        Timer m_Timer;
        std::queue<Instruction> m_Instructions;
        bool m_Ready;
    };
}

#endif
