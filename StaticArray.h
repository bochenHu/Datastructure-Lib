#ifndef STATICARRAY_H
#define STATICARRAY_H

#include "Array.h"

namespace HBCLib
{

template <typename T, int N>
class StaticArray: public Array<T>
{
protected:
    T m_space[N];
public:
    StaticArray()
    {
        Array<T>::m_array = m_space;
    }

    StaticArray(const StaticArray<T, N>& t)
    {
        Array<T>::m_array = m_space;

        for (int i = 0; i < length(); ++i)
        {
            Array<T>::m_array[i] = t.m_space[i];
        }
    }

    StaticArray<T,N>& operator=(const StaticArray<T, N>& t)
    {
        if (this != &t)
        {
            Array<T>::m_array = m_space;

            for (int i = 0; i < length(); ++i)
            {
                Array<T>::m_array[i] = t.m_space[i];
            }
        }
        return *this;
    }

    int length() const
    {
        return N;
    }
};
}

#endif // STATICARRAY_H
