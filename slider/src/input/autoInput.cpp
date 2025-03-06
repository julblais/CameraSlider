#include "autoInput.h"

using namespace Input;
using namespace Core;

AutoInput::AutoInput(std::initializer_list<InputInstruction> instructions)
    : m_Instructions(instructions), m_Ready(true)
{
    m_Timer = Timer::Create("Auto-input", [this]() {
        m_Ready = true;
        m_Instructions.pop();
    });
}

Input::AutoInput::AutoInput(const unsigned int interval, std::initializer_list<InputInstruction> instructions)
    : m_Instructions(), m_Ready(true)
{
    for (auto cmd : instructions)
    {
        m_Instructions.push(PauseInstr(interval));
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

    auto instruction = m_Instructions.front();

    if (instruction.command == InputInstruction::Command::Pause)
    {
        if (m_Ready)
        {
            m_Ready = false;
            m_Timer.Start(instruction.durationMs);
        }
    }
    else if (instruction.command == InputInstruction::Command::Hold)
    {
        if (m_Ready)
        {
            m_Ready = false;
            m_Timer.Start(instruction.durationMs);
        }
        return instruction.input;
    }
    else if (instruction.command == InputInstruction::Command::Input)
    {
        m_Instructions.pop();
        return instruction.input;
    }

    return InputData();
}
