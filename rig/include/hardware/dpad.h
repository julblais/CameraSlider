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

    class IDpad {
        public:
            IDpad();
            virtual ~IDpad(){}
            void AddListener(IDpadListener* listener);
            void RemoveListener(IDpadListener* listener);
            void SendUpEvent(const DpadButton button) const;
            void SendDownEvent(const DpadButton button) const;
            static const char* PrintButton(const DpadButton button);

            virtual void Init() =0;
            virtual void Update() =0;

        protected:
            std::vector<IDpadListener*> m_Listeners;
    };
}

#endif