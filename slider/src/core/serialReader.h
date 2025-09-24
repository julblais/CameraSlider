#ifndef SERIAL_READER_H
#define SERIAL_READER_H

#include <functional>
#include <vector>

namespace IO
{
    class SerialReader
    {
        using TListener = std::function<bool(TArgs...)>;

    public:
        SerialReader() = default;
        void AddListener(TListener listener);

    private:
        std::vector<TListener> m_Listeners;
    };
}
