#ifndef LCDPRINT_H
#define LCDPRINT_H

#include <array>
#include <Print.h>
#include <HardwareSerial.h>

class LCDPRint : public Print 
{
    constexpr static auto LCD_LINE_LENGTH = 16;
    constexpr static auto LCD_NUM_LINES = 2;

    using Keycode = uint8_t;

    public:
        LCDPRint() = default;
        virtual size_t write(Keycode value) override
        {
            Serial.println(value);
            return 1;
        }

        //set cursor
        //handle custom chars
        

    private:
        std::array<Keycode, LCD_LINE_LENGTH * LCD_NUM_LINES> m_Buffer;

        //idea is: accumulate the values in a buffer
        //then send to lcd (+ send message) at the end of the frame
};

#endif