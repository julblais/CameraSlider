#ifndef CTRL_H
#define CTRL_H

namespace Hardware{

    class Joystick
    {
        public:
        Joystick(const int verticalPin, const int horizontalPin, const int selectionPin);

        void Init();
        int ReadVertical();
        int ReadHorizontal();
        bool SelectionPressed();

        private:
        int m_VerticalPin;
        int m_HorizontalPin;
        int m_SelectionPin;
    };
}

#endif