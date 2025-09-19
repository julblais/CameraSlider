#ifndef AUTOINPUT_H
#define AUTOINPUT_H

#include <initializer_list>
#include <queue>
#include "core/time/time.h"
#include "core/timer.h"
#include "event.h"

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

        constexpr Instruction(const Core::Time durationMs, const InputData& input) :
            durationMs(durationMs),
            input(input),
            type(Type::Hold)
        {}

        constexpr Instruction(const InputData& input) :
            durationMs(0),
            input(input),
            type(Type::Input)
        {}

        constexpr Instruction(const Core::Time durationMs) :
            durationMs(durationMs),
            input(),
            type(Type::Pause)
        {}

        const Core::Time durationMs;
        const InputData input;
        const Type type;

        static constexpr Instruction Pause(Core::Time durationMs) { return { durationMs }; }

        static constexpr Instruction DpadUp() { return {{ButtonDpadUp}}; }
        static constexpr Instruction DpadDown() { return {{ButtonDpadDown}}; }
        static constexpr Instruction DpadLeft() { return {{ButtonDpadLeft}}; }
        static constexpr Instruction DpadRight() { return {{ButtonDpadRight}}; }
        static constexpr Instruction DpadSelect() { return {{ButtonSelect}}; }

        static constexpr Instruction DpadUp(Core::Time hold) { return {hold, {ButtonDpadUp}}; }
        static constexpr Instruction DpadDown(Core::Time hold) { return {hold, {ButtonDpadDown}}; }
        static constexpr Instruction DpadLeft(Core::Time hold) { return {hold, {ButtonDpadLeft}}; }
        static constexpr Instruction DpadRight(Core::Time hold) { return {hold, {ButtonDpadRight}}; }
        static constexpr Instruction DpadSelect(Core::Time hold) { return {hold, {ButtonSelect}}; }

        static constexpr Instruction Joystick(float x, float y) { return { { x, y } }; }
        static constexpr Instruction Joystick(float x, float y, Core::Time hold) { return { hold, { x, y } }; }
    };

    class AutoInput
    {
    public:
        AutoInput(const std::initializer_list<Instruction> instructions);
        AutoInput(const unsigned int interval, const std::initializer_list<Instruction> instructions);
        ~AutoInput() = default;
        InputData ReadInput();

    private:
        Core::Timer m_Timer;
        std::queue<Instruction> m_Instructions;
        bool m_Ready;
    };
}

#endif
