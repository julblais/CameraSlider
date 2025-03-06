#include "autoInput.h"

using namespace Input;
using namespace Core;

AutoInput::AutoInput(std::initializer_list<Command> commands)
    : m_Commands(commands), m_Ready(true)
{
    m_Timer = Timer::Create("Auto-input", [this]() {
        m_Ready = true;
        m_Commands.pop();
    });
}

Input::AutoInput::AutoInput(const unsigned int interval, std::initializer_list<Command> commands)
    : m_Commands(), m_Ready(true)
{
    for (auto cmd : commands)
    {
        m_Commands.push(CommandCreator::Pause(interval));
        m_Commands.push(cmd);
    }
    m_Timer = Timer::Create("Auto-input", [this]() {
        m_Ready = true;
        m_Commands.pop();
    });
}

InputData Input::AutoInput::ReadInput()
{
    if (m_Commands.empty())
        return InputData();

    auto command = m_Commands.front();

    if (command.instruction == Command::Instruction::Pause)
    {
        if (m_Ready)
        {
            m_Ready = false;
            m_Timer.Start(command.durationMs);
        }
    }
    else if (command.instruction == Command::Instruction::Hold)
    {
        if (m_Ready)
        {
            m_Ready = false;
            m_Timer.Start(command.durationMs);
        }
        return command.input;
    }
    else if (command.instruction == Command::Instruction::Input)
    {
        m_Commands.pop();
        return command.input;
    }

    return InputData();
}