#include "connectionCommand.h"

using namespace Core;
using namespace Slider;

ConnectionCommand::ConnectionCommand(BrainConnector* connector) :
    m_Connector(connector),
    m_AnimPrint(500, {" ", ".", "..", "..."}) {}

void ConnectionCommand::Print(Display* display) const
{
    PrintTitle(display, "Manette");
}

void ConnectionCommand::Invoke(const MenuCommandButton command) {}

void ConnectionCommand::OnShow() {}

void ConnectionCommand::OnHide() {}
