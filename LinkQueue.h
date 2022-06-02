#ifndef LINKQUEUE_H
#define LINKQUEUE_H

#include "exception.h"
#include "Queue.h"
#include "DualCircleLinkList.h"

namespace HBCLib
{

template <typename T>
class LinkQueue: public Queue<T>
{
protected:
    DualCircleLinkList<T> m_list;
public:
    void enqueue(const T& t)
    {
        m_list.insert(t);
    }

    void dequeue()
    {
        if (m_list.length() > 0)
        {
            m_list.remove(0);
        }
        else
        {
            EXCEPTION_THROW(InvalidOperationException,"no element in current queue...");
        }
    }

    void clear()
    {
        m_list.clear();
    }

    int length() const
    {
        return m_list.length();
    }

    T front() const
    {
        return m_list.get(0);
    }
};

}

#endif // LINKQUEUE_H
