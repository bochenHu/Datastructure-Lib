#ifndef SEQLIST_H
#define SEQLIST_H

#include "List.h"
#include "exception.h"

namespace HBCLib
{

template <typename T>
class SeqList: public List<T>
{
protected:
    T* m_array;
    int m_length;
public:
    bool insert(int i, const T &t)
    {
        bool ret = ((0 <= i) && (i < m_length+1));
        ret = ret && (m_length + 1 <= capacity());

        if (ret)
        {
            for (int p = m_length - 1; p >= i; --p)
            {
                m_array[p+1] = m_array[p];
            }

            m_array[i] = t;
            m_length++;
            return ret;
        }
        else
        {
            EXCEPTION_THROW(IndexOutOfBoundsException,"invalid argument i...");
        }
    }

    bool insert(const T& t)
    {
        return insert(m_length,t);
    }

    bool remove(int i)
    {
        bool ret = ((0 <= i) && (i < m_length));

        if (ret)
        {
            for (int p = i; p < m_length-1; ++p)
            {
                m_array[p] = m_array[p+1];
            }

            m_length--;
            return ret;
        }
        else
        {
            EXCEPTION_THROW(IndexOutOfBoundsException,"invalid argument i...");
        }
    }

    void clear()
    {
        m_length = 0;
    }

    bool get(int i, T &t) const
    {
        bool ret = ((0 <= i) && (i < m_length));

        if(ret)
        {
            t = m_array[i];
            return ret;
        }
        else
        {
            EXCEPTION_THROW(IndexOutOfBoundsException,"invalid argument i...");
        }
    }

    T get(int i) const
    {
        T ret;

        if (get(i,ret))
        {
            return ret;
        }
        else
        {
            EXCEPTION_THROW(IndexOutOfBoundsException,"invalid argument i...");
        }
    }

    int length() const
    {
        return m_length;
    }

    int find(const T& t) const
    {
        int ret = -1;
        for (int i = 0; i < m_length; ++i)
        {
            if (m_array[i] == t)
            {
                ret = i;
                break;
            }
        }
        return ret;
    }

    bool set(int i, const T &t)
    {
        bool ret = ((0 <= i) && (i < m_length));

        if(ret)
        {
            m_array[i] = t;
            return ret;
        }
        else
        {
            EXCEPTION_THROW(IndexOutOfBoundsException,"invalid argument i...");
        }
    }
    T& operator[](int i)
    {
        if ((0 <= i) && (i < m_length))
        {
            return m_array[i];
        }
        else
        {
            EXCEPTION_THROW(IndexOutOfBoundsException,"invalid argument i...");
        }
    }
    T operator[](int i) const
    {
        return (const_cast<SeqList<T>&> (*this))[i];
    }

    virtual int capacity() const = 0;
};

}

#endif // SEQLIST_H
