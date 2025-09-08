#include "addressCommand.h"
#include "src/app/settings.h"

using namespace Core;
using namespace Slider;

void BrainAddressCommand::Print(Display* display) const
{
    PrintTitle(display, "Adr. mac");
}

void ControllerAddressCommand::Print(Display* display) const
{
    PrintTitle(display, "Adr. manette");
    if (Settings::GetInstance().HasPeerAddress())
        display->PrintLine(1, "  ", Settings::GetInstance().GetPeerAddress());
    else
        display->PrintLine(1, "  Aucune manette");
}
