#include "serialReader.h"

#include "debug.h"
#include "HardwareSerial.h"
#include "WString.h"

using namespace Core;

SerialReader::SerialReader() {}

void SerialReader::ProcessInput() const
{
    /*
        if (Serial.available())
        {
            const auto input = Serial.readStringUntil('\n');
            LogInfo("Received message: ", input);
            for (const auto& listener : m_Listeners)
            {
                if (listener(input.c_str()))
                    break;
            }
        }*/
}

void SerialReader::AddListener(TListener listener)
{
    m_Listeners.push_back(listener);
}
