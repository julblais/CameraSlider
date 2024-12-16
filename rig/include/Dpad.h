#ifndef CTRL_H
#define CTRL_H

#include <vector>

namespace Hardware{

    enum DpadButton : char {
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

    class Dpad {
        struct State {
            public:
                int Vertical;
                int Horizontal;
                bool Selection;

                State(const int vertical, const int horizontal, const int selection);
                bool IsLeft() const;
                bool IsRight() const;
                bool IsUp() const;
                bool IsDown() const;
        };

        public:
        Dpad(const int verticalPin, const int horizontalPin, const int selectionPin);

        void Init();
        int ReadVertical() const;
        int ReadHorizontal() const;
        bool SelectionPressed() const;

        void Update();
        void AddListener(IDpadListener* listener);
        void RemoveListener(IDpadListener* listener);
        const char* PrintButton(const DpadButton button) const;

        private:
        State ReadState() const;
        void SendUpEvent(const DpadButton button) const;
        void SendDownEvent(const DpadButton button) const;
        void ProcessButtons(const State& state) const;

        int m_VerticalPin;
        int m_HorizontalPin;
        int m_SelectionPin;
        State m_LastState;
        
        std::vector<IDpadListener*> m_Listeners;
    };
}

#endif