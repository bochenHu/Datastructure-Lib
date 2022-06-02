#ifndef STATICSTACK_H
#define STATICSTACK_H

#include "Stack.h"
#include "exception.h"

namespace HBCLib
{

template <typename T, int N>
class StaticStack: public Stack<T>
{
protected:
    T m_space[N];
    int m_top;
    int m_size;
public:
    StaticStack()
    {
        m_top = -1;
        m_size = 0;
    }

    void push(const T& t)
    {
        if (m_size < N)
        {
            m_space[m_top + 1] = t;
            m_top++;
            m_size++;
        }
        else
        {
            EXCEPTION_THROW(InvalidOperationException,"no space in current stack...");
        }
    }

    void pop()
    {
        if (m_size > 0)
        {
            m_top--;
            m_size--;
        }
        else
        {
            EXCEPTION_THROW(InvalidOperationException,"no element in current stack...");
        }
    }

    void clear()
    {
        m_top = -1;
        m_size = 0;
    }

    int size() const
    {
        return m_size;
    }

    T top() const
    {
        if (m_size > 0)
        {
            return m_space[m_top];
        }
        else
        {
            EXCEPTION_THROW(InvalidOperationException,"no element in current stack...");
        }
    }

    int capacity() const
    {
        return N;
    }

    ~StaticStack()
    {
        clear();
    }
};

}

#endif // STATICSTACK_H
