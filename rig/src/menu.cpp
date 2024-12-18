#include "menu.h"
#include "hardware/lcd.h"
#include "hardware/dpad.h"
#include "debug.h"
#include "utils/timer.h"

#define MENU_DELAY_MS 2000

class SpeedMaxCommand : public Utils::MenuCommand
{
    public:
        enum Speed  {
            SLOW = 0,
            SMED = 1,
            SHIGH = 2
        };

        SpeedMaxCommand() : m_Speed(Speed::SLOW) {}
        virtual ~SpeedMaxCommand() {}

        virtual const char* GetTitle() override
        {
            return "Vitesse max.";
        }
        
        virtual const char* GetDesc() override
        {
            if (m_Speed == Speed::SLOW)
                return "Pas vite";
            else if (m_Speed == Speed::SMED)
                return "Moyen vite";
            else if (m_Speed == Speed::SHIGH)
                return "Fuckin vite";
            else return nullptr;
        }

        virtual void Invoke(Utils::MenuCommandButton command) override
        {
            if (m_Speed == Speed::SLOW)
                m_Speed = Speed::SMED;
            else if (m_Speed == Speed::SMED)
                m_Speed = Speed::SHIGH;
            else if (m_Speed == Speed::SHIGH)
                m_Speed = Speed::SLOW;
        }

    private:
        Speed m_Speed;
};

class SpeedCurveCommand : public Utils::MenuCommand
{
    public:
        enum Curve  {
            CLINEAR = 0,
            CQUAD = 1,
            CEXP = 2
        };

        SpeedCurveCommand() {}
        virtual ~SpeedCurveCommand() {}

        virtual const char* GetTitle() override
        {
            return "Courbe vitesse";
        }
        
        virtual const char* GetDesc() override
        {
            if (m_Curve == Curve::CLINEAR)
                return "Lineaire";
            else if (m_Curve == Curve::CQUAD)
                return "Quadratique";
            else if (m_Curve == Curve::CEXP)
                return "Exponentielle";
            else return nullptr;
        }

        virtual void Invoke(Utils::MenuCommandButton command) override
        {
            if (m_Curve == Curve::CLINEAR)
                m_Curve = Curve::CQUAD;
            else if (m_Curve == Curve::CQUAD)
                m_Curve = Curve::CEXP;
            else if (m_Curve == Curve::CEXP)
                m_Curve = Curve::CLINEAR;
        }

    private:
        Curve m_Curve;
};

Rig::Menu::Menu(Hardware::LCD* lcd, Hardware::IDpad* dpad) :
    m_LCD(lcd),
    m_Dpad(dpad),
    m_Timer("Selection menu", [this](unsigned long time){ OnOpenMenu(time); }, MENU_DELAY_MS),
    m_MenuSystem()
{
}

void Rig::Menu::Init()
{
    m_Dpad->AddListener(this);
    
    m_MenuSystem.AddCommand(new SpeedMaxCommand());
    m_MenuSystem.AddCommand(new SpeedCurveCommand());
}

void Rig::Menu::OnKeyUp(const Hardware::DpadButton button)
{
    m_Timer.Stop();
}

void Rig::Menu::OnKeyDown(const Hardware::DpadButton button)
{
    if(button == Hardware::DpadButton::SELECTION)
        m_Timer.Start();
    else if(button == Hardware::DpadButton::LEFT)
        m_MenuSystem.MoveLeft();
    else if(button == Hardware::DpadButton::RIGHT)
        m_MenuSystem.MoveRight();
    else if(button == Hardware::DpadButton::UP)
        m_MenuSystem.MoveUp();
    else if(button == Hardware::DpadButton::DOWN)
        m_MenuSystem.MoveDown();

    OutputMenu();
}

void Rig::Menu::OnOpenMenu(unsigned long time)
{
    Debug.Log("Open menu! ", time);
    m_LCD->Clear();
    m_LCD->PrintLn("  -- Menu -- ", 0);
    delay(1500);
    m_MenuSystem.Open();
    OutputMenu();
}

void Rig::Menu::OutputMenu()
{
    if (m_MenuSystem.IsOpened())
    {
        auto out = m_MenuSystem.GetOutput();
        m_LCD->PrintLn(m_LCD->GetDoubleUpDownArrows(), out.title, 0);
        m_LCD->PrintLn(" ", m_LCD->GetDoubleLeftRightArrows(), out.desc, 1);
    }
}
