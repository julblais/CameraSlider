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

            bool IsLeft() const;
            bool IsRight() const;
            bool IsUp() const;
            bool IsDown() const;
        };

        public:
        Dpad(const int verticalPin, const int horizontalPin, const int selectionPin);

        void Init();
        int ReadVertical();
        int ReadHorizontal();
        bool SelectionPressed();

        void Update();
        void AddListener(IDpadListener* listener);
        void RemoveListener(IDpadListener* listener);
        const char* PrintButton(const DpadButton button);

        private:
        State ReadState();
        void SendUpEvent(const DpadButton button);
        void SendDownEvent(const DpadButton button);
        void ProcessButtons(const State& state);

        int m_VerticalPin;
        int m_HorizontalPin;
        int m_SelectionPin;
        State m_LastState;
        
        std::vector<IDpadListener*> m_Listeners;
    };
}

#endif