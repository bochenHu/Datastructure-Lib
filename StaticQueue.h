#ifndef STATICQUEUE_H
#define STATICQUEUE_H

#include "Queue.h"
#include "exception.h"

namespace HBCLib
{

template <typename T, int N>
class StaticQueue: public Queue<T>
{
protected:
    T m_space[N];
    int m_front;
    int m_rear;
    int m_size;
public:
    StaticQueue()
    {
        m_front = 0;
        m_rear = 0;
        m_size = 0;
    }

    void enqueue(const T& t)
    {
        if (m_size < N)
        {
            m_space[m_rear] = t;
            m_rear = (m_rear + 1) % N;
            m_size++;
        }
        else
        {
            EXCEPTION_THROW(InvalidOperationException,"no space in current queue...");
        }
    }

    void dequeue()
    {
        if (m_size > 0)
        {
            m_front = (m_front + 1) % N;
            m_size--;
        }
        else
        {
            EXCEPTION_THROW(InvalidOperationException,"no element in current queue...");
        }
    }

    void clear()
    {
        m_front = 0;
        m_rear = 0;
        m_size = 0;
    }

    T front() const
    {
        if (m_size > 0)
        {
            return m_space[m_front];
        }
        else
        {
            EXCEPTION_THROW(InvalidOperationException,"no element in current queue...");
        }
    }

    int length() const
    {
        return m_size;
    }

    int capacity() const
    {
        return N;
    }

    ~StaticQueue()
    {
        clear();
    }
};

}

#endif // STATICQUEUE_H
