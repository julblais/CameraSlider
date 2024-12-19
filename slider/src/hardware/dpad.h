#ifndef CTRL_H
#define CTRL_H

#include "src/utils/eventSource.h"

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

    class IDpad : public Utils::EventSource<IDpadListener> {
    
        struct State {
            public:
                DpadButton button;

                inline bool IsDown() const { return button == DpadButton::DOWN; }
                inline bool IsUp() const { return button == DpadButton::UP; }
                inline bool IsLeft() const { return button == DpadButton::LEFT; }
                inline bool IsRight() const { return button == DpadButton::RIGHT; }
                inline bool IsSelection() const { return button == DpadButton::SELECTION; }

                State(){}
                State(DpadButton button);
        };

        public:
            IDpad();
            virtual ~IDpad(){}
            static const char* PrintButton(const DpadButton button);

            virtual void Init();
            virtual void Update();
            virtual DpadButton ReadButton() const =0;

        protected:
            State m_LastState;
        
        private:
            void SendUpEvent(const DpadButton button) const;
            void SendDownEvent(const DpadButton button) const;
            void ProcessButton(const State& state) const;
    };
}

#endif