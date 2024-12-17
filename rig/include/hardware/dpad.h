#ifndef CTRL_H
#define CTRL_H

#include <vector>

namespace Hardware{

    enum DpadButton : char {
        NONE = 0,
        UP = 1,
        DOWN = 2,
        LEFT = 4,
        RIGHT = 8,
        SELECTION = 16
    };

    class IDpadListener {
        public:
            virtual ~IDpadListener() {}

            virtual void OnKeyUp(const DpadButton button) {}
            virtual void OnKeyDown(const DpadButton button) {}
    };

    class IDpad {
    
        struct State {
            public:
                DpadButton button;
                int delta;

                inline bool IsDown() const { return button == DpadButton::DOWN; }
                inline bool IsUp() const { return button == DpadButton::UP; }
                inline bool IsLeft() const { return button == DpadButton::LEFT; }
                inline bool IsRight() const { return button == DpadButton::RIGHT; }
                inline bool IsSelection() const { return button == DpadButton::SELECTION; }

                State(){}
                State(DpadButton button, int delta);
        };

        public:
            IDpad();
            virtual ~IDpad(){}
            void AddListener(IDpadListener* listener);
            void RemoveListener(IDpadListener* listener);
            void SendUpEvent(const DpadButton button) const;
            void SendDownEvent(const DpadButton button) const;
            static const char* PrintButton(const DpadButton button);

            virtual void Init();
            virtual void Update();
            virtual DpadButton ReadButton() const =0;

        protected:
            std::vector<IDpadListener*> m_Listeners;
            State m_LastState;
        
        private:
            State CreateState(DpadButton button) const;
            void ProcessButton(const State& state) const;
    };
}

#endif