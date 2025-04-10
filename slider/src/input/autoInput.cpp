#include "autoInput.h"

using namespace Input;
using namespace Core;

AutoInput::AutoInput(std::initializer_list<Instruction> instructions)
    : m_Instructions(instructions), m_Ready(true)
{
    m_Timer = Timer::Create("Auto-input", [this]() {
        m_Ready = true;
        m_Instructions.pop();
    });
}

Input::AutoInput::AutoInput(const unsigned int interval, std::initializer_list<Instruction> instructions)
    : m_Instructions(), m_Ready(true)
{
    for (auto cmd : instructions)
    {
        m_Instructions.push(Instruction::Pause(interval));
        m_Instructions.push(cmd);
    }
    m_Timer = Timer::Create("Auto-input", [this]() {
        m_Ready = true;
        m_Instructions.pop();
    });
}

InputData Input::AutoInput::ReadInput()
{
    if (m_Instructions.empty())
        return InputData();

    auto command = m_Instructions.front();

    if (command.type == Instruction::Type::Pause)
    {
        if (m_Ready)
        {
            m_Ready = false;
            m_Timer.Start(command.durationMs);
        }
    }
    else if (command.type == Instruction::Type::Hold)
    {
        if (m_Ready)
        {
            m_Ready = false;
            m_Timer.Start(command.durationMs);
        }
        return command.input;
    }
    else if (command.type == Instruction::Type::Input)
    {
        m_Instructions.pop();
        return command.input;
    }

    return InputData();
}