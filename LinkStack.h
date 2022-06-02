#ifndef LINKSTACK_H
#define LINKSTACK_H

#include "Stack.h"
#include "LinkList.h"

namespace HBCLib
{

template <typename T>
class LinkStack: public Stack<T>
{
protected:
    LinkList<T> m_list;
public:
    void push(const T& t)
    {
        m_list.insert(0,t);
    }

    void pop()
    {
        m_list.remove(0);
    }

    void clear()
    {
        m_list.clear();
    }

    int size() const
    {
        return m_list.length();
    }

    T top() const
    {
        return m_list.get(0);
    }
};

}

#endif // LINKSTACK_H
