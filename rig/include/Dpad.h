#ifndef CTRL_H
#define CTRL_H

#include <vector>

namespace Hardware{

    class IJoystickListener {

        public:

        void OnSelection() {}
        void OnMoveUp() {}
        void OnMoveDown() {}
        void OnMoveLeft() {}
        void OnMoveRight() {}
    };

    class Dpad
    {
        public:
        Dpad(const int verticalPin, const int horizontalPin, const int selectionPin);

        void Init();
        int ReadVertical();
        int ReadHorizontal();
        bool SelectionPressed();

        private:
        int m_VerticalPin;
        int m_HorizontalPin;
        int m_SelectionPin;
        std::vector<IJoystickListener> m_Events;
    };
}

#endif