#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include "Array.h"
#include "exception.h"

namespace HBCLib
{

template <typename T>
class DynamicArray:public Array<T>
{
protected:
    int m_length;

    T* copy(T* array, int len, int newlen)
    {
        T* ret = new T[newlen];

        if (ret)
        {
            int size = (len < newlen) ? len : newlen;

            for (int i = 0; i < size; ++i)
            {
                ret[i] = array[i];
            }
        }
        return ret;
    }

    void update(T* array, int length)
    {
        if (array)
        {
            T* temp = this -> m_array;

            this -> m_array = array;
            m_length = length;

            delete[] temp;
        }
        else
        {
            EXCEPTION_THROW(NoEnoughMemoryException,"no enough memory to update dynamicarray object...");
        }
    }

    void init(T* array, int length)
    {
        if (array)
        {
            this -> m_array = array;
            m_length = length;
        }
        else
        {
            EXCEPTION_THROW(NoEnoughMemoryException,"no enough memory to initiate dynamicarray object...");
        }
    }

public:
    DynamicArray(int length = 0)
    {
        init(new T[length],length);
    }

    DynamicArray(const DynamicArray<T>& t)
    {
        init(copy(t.m_array,t.m_length,t.m_length),t.length());
    }

    DynamicArray<T>& operator=(const DynamicArray<T>& t)
    {
        if (this != &t)
        {
            update(copy(t.m_array,t.m_length,t.m_length),t.m_length);
        }
        return * this;
    }

    void resize(int length)
    {
        update(copy(this -> m_array, m_length, length), length);
    }

    int length() const
    {
        return m_length;
    }

    ~DynamicArray()
    {
        delete[] this->m_array;
    }
};
}

#endif // DYNAMICARRAY_H
