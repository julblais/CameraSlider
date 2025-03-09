#ifndef VECTORUTILS_H
#define VECTORUTILS_H

#include <vector>

namespace Core
{
    template <class TElement, class T>
    void RemoveAll(std::vector<TElement>& vector, const T& value)
    {
        auto t = std::find(vector.begin(), vector.end(), value);
        while (t != vector.end())
        {
            vector.erase(t);
            t = std::find(vector.begin(), vector.end(), value);
        }
    }

    template <class TElement, class T>
    void RemoveFirst(std::vector<TElement>& vector, const T& value)
    {
        auto t = std::find(vector.begin(), vector.end(), value);
        if (t != vector.end())
            vector.erase(t);
    }
}

#endif