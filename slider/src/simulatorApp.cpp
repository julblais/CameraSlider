#if IS_SIMULATOR

#include "simulatorApp.h"
#include "deviceInputReader.h"
#include "core/perf.h"
#include "menu.h"
#include "stepper.h"
#include "autoInput.h"
#include "settingCommand.h"
#include "lcd.h"
#include "core/animatedPrintable.h"

using namespace IO;
using namespace Core;
using namespace Hardware;

class TestActionCommand : public MenuCommand
{
public:
    void Print(Display& display) const override
    {
        TitlePrefix(display) << "Test Action" << Endl;
        DescriptionPrefix(display, DescriptionType::Action) << "Action" << m_Progress << Endl;
    }

    void Invoke(const Button command) override
    {
        if (command == ButtonSelect)
            LogInfo("Action invoked!");
    }
private:
    AnimatedPrintable m_Progress = AnimatedPrintable::CreateProgressDots(500);
};

class TestOptionsCommand : public MenuCommand
{
    enum class Option
    {
        Option1,
        Option2,
        Option3,
        COUNT
    };

public:
    void Print(Display& display) const override
    {
        TitlePrefix(display) << "Test Option" << Endl;

        switch (m_Option)
        {
        case Option::Option1:
            DescriptionPrefix(display, DescriptionType::Options) << "Option 1" << Endl;
            break;
        case Option::Option2:
            DescriptionPrefix(display, DescriptionType::Options) << "Option 2" << Endl;
            break;
        case Option::Option3:
            DescriptionPrefix(display, DescriptionType::Options) << "Option 3" << Endl;
            break;
        default:
            DescriptionPrefix(display, DescriptionType::Options) << "ERROR" << Endl;
            break;
        }
    }

    void Invoke(const Button command) override
    {
        if (command == ButtonLeft)
            m_Option = GetPreviousEnumValue(m_Option);
        else if (command == ButtonRight)
            m_Option = GetNextEnumValue(m_Option);
    }

private:
    Option m_Option = Option::Option1;
};

Slider::SimulatorApp::SimulatorApp(const AppConfig& config) :
    m_Config(config)
{}

bool Slider::SimulatorApp::OnInputEvent(const Event& inputEvent)
{
    if (inputEvent.HasChange())
    {
        m_DisplayBuffer.Clear();
        m_DisplayBuffer.Print(inputEvent);
    }
    return false;
}

void Slider::SimulatorApp::Setup()
{
    m_Display = std::unique_ptr<Display>(new LCD(m_Config.LcdAddress));
    m_DisplayBuffer.Setup(m_Display.get());

    InputPins pins {
        .dpadUp = m_Config.DpadUpPin,
        .dpadDown = m_Config.DpadDownPin,
        .dpadLeft = m_Config.DpadLeftPin,
        .dpadRight = m_Config.DpadRightPin,
        .dpadSelection = m_Config.DpadSelectionPin,
        .joystickHorizontal = m_Config.JoystickXPin,
        .joystickVertical = m_Config.JoystickYPin,
        .joystickCenter = m_Config.JoystickCenterPin,
    };
    m_LocalInput = std::unique_ptr<DeviceInputReader>(new DeviceInputReader(pins));
    m_LocalInput->Setup();

    m_AutoInput = std::unique_ptr<AutoInput>(new AutoInput(2700, {
                                                               Instruction::DpadSelect(2500),
                                                               Instruction::DpadDown(),
                                                               Instruction::DpadDown(),
                                                               Instruction::DpadLeft(),
                                                               Instruction::DpadSelect(2500),
                                                               Instruction::Joystick(0.5f, 0.4f, 5000)
        }));

    AddComponent<TimerComponent>();
    const auto menu = AddComponent<Menu>(&m_DisplayBuffer, m_Config.ShowMenuDelayMs);
    const auto stepper = AddComponent<Stepper>(m_Config.StepperDirectionPin, m_Config.StepperStepPin);
    SetupComponents();

    menu->AddCommand(new TestActionCommand());
    menu->AddCommand(new TestOptionsCommand());
    menu->AddCommand(new MaxSpeedCommand());
    menu->AddCommand(new SpeedCurveCommand());

    m_InputDispatcher.AddListener(menu);
    m_InputDispatcher.AddListener(stepper);
    m_InputDispatcher.AddListener(this);

    m_DisplayBuffer.Print(Event());
}

void Slider::SimulatorApp::Update()
{
    const auto localInput = m_LocalInput->ReadInput();
    //const auto autoInput = m_AutoInput->ReadInput();
    m_InputDispatcher.ProcessInput(localInput);
    //m_InputDispatcher.ProcessInput(autoInput);
    m_InputDispatcher.Dispatch();

    UpdateComponents();

    //output final display buffer
    m_DisplayBuffer.PrintToDisplay();
}

#endif
