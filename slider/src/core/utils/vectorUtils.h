#ifndef VECTORUTILS_H
#define VECTORUTILS_H

#include <vector>
#include <algorithm>

namespace Core
{
    template<class TElement, class T>
    void EraseAll(std::vector<TElement>& vector, const T& value)
    {
        auto t = std::find(vector.begin(), vector.end(), value);
        while (t != vector.end())
        {
            vector.erase(t);
            t = std::find(vector.begin(), vector.end(), value);
        }
    }

    template<class TElement, class TPredicate>
    void EraseAllIf(std::vector<TElement>& vector, TPredicate pred)
    {
        auto t = std::find_if(vector.begin(), vector.end(), pred);
        while (t != vector.end())
        {
            vector.erase(t);
            t = std::find(vector.begin(), vector.end(), pred);
        }
    }

    template<class TElement, class T>
    bool EraseFirst(std::vector<TElement>& vector, const T& value)
    {
        auto t = std::find(vector.begin(), vector.end(), value);
        if (t != vector.end())
        {
            vector.erase(t);
            return true;
        }
        return false;
    }

    template<class TElement, class TPredicate>
    bool EraseFirstIf(std::vector<TElement>& vector, TPredicate pred)
    {
        auto t = std::find_if(vector.begin(), vector.end(), pred);
        if (t != vector.end())
        {
            vector.erase(t);
            return true;
        }
        return false;
    }
}

#endif
