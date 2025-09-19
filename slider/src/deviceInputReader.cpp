#include "deviceInputReader.h"
#include "core/debug.h"
#include "core/utils/mathUtils.h"

using namespace Hardware;
using namespace IO;

constexpr auto JOYSTICK_RANGE_X = 4096;
constexpr auto JOYSTICK_RANGE_Y = 4096;

DeviceInputReader::DeviceInputReader(const InputPins& pins)
    : m_Pins(pins)
{}

DeviceInputReader::~DeviceInputReader() {}

void DeviceInputReader::Setup()
{
    pinMode(m_Pins.dpadUp, INPUT_PULLUP);
    pinMode(m_Pins.dpadDown, INPUT_PULLUP);
    pinMode(m_Pins.dpadLeft, INPUT_PULLUP);
    pinMode(m_Pins.dpadRight, INPUT_PULLUP);
    pinMode(m_Pins.dpadSelection, INPUT_PULLUP);
    pinMode(m_Pins.joystickVertical, INPUT);
    pinMode(m_Pins.joystickHorizontal, INPUT);
    pinMode(m_Pins.joystickCenter, INPUT_PULLUP);
}

IO::InputData DeviceInputReader::ReadInput()
{
    auto up = digitalRead(m_Pins.dpadUp) == LOW;
    auto down = digitalRead(m_Pins.dpadDown) == LOW;
    auto left = digitalRead(m_Pins.dpadLeft) == LOW;
    auto right = digitalRead(m_Pins.dpadRight) == LOW;
    auto selection = digitalRead(m_Pins.dpadSelection) == LOW;
    auto v = analogRead(m_Pins.joystickVertical);
    auto h = analogRead(m_Pins.joystickHorizontal);
    auto joystickSelection = digitalRead(m_Pins.joystickCenter) == LOW;

    auto buttons = ButtonNone;
    if (left)
        buttons |= ButtonDpadLeft;
    if (right)
        buttons |= ButtonDpadRight;
    if (up)
        buttons |= ButtonDpadUp;
    if (down)
        buttons |= ButtonDpadDown;
    if (selection)
        buttons |= ButtonSelect;
    if (joystickSelection)
        buttons |= ButtonCenter;

    auto x = -Core::Remap((float)h, 0.0f, (float)JOYSTICK_RANGE_X, -1.0f, 1.0f);
    auto y = Core::Remap((float)v, 0.0f, (float)JOYSTICK_RANGE_Y, -1.0f, 1.0f);

    return IO::InputData(buttons, x, y);
}