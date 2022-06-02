#ifndef ARRAY_H
#define ARRAY_H

#include "object.h"
#include "exception.h"

namespace HBCLib
{

template <typename T>
class Array: public Object
{
protected:
    T* m_array;
public:
    virtual bool get(int i, T& t) const
    {
        bool ret = ((0 <= i) && (i < length()));

        if (ret)
        {
            t = m_array[i];
            return ret;
        }
        else
        {
            EXCEPTION_THROW(IndexOutOfBoundsException,"invalid argument i...");
        }
    }

    virtual int length() const = 0;

    virtual bool set(int i, const T& t)
    {
        bool ret = ((0 <= i) && (i < length()));

        if (ret)
        {
            m_array[i] = t;
            return ret;
        }
        else
        {
            EXCEPTION_THROW(IndexOutOfBoundsException,"invalid argument i...");
        }
    }

    virtual T& operator[](int i)
    {
        bool ret = ((0 <= i) && (i < length()));

        if (ret)
        {
            return m_array[i];
        }
        else
        {
            EXCEPTION_THROW(IndexOutOfBoundsException,"invalid argument i...");
        }
    }

    T* array() const
    {
        return m_array;
    }

    virtual T operator[](int i) const
    {
        return (const_cast<Array<T>&> (*this))[i];
    }
};
}

#endif // ARRAY_H
