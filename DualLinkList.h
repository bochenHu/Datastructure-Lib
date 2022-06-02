#ifndef DUALLINKLIST_H
#define DUALLINKLIST_H

#include "List.h"
#include "exception.h"

namespace HBCLib
{

template <typename T>
class DualLinkList: public List<T>
{
protected:
    struct Node: public Object
    {
        T value;
        Node* next;
        Node* pre;
    };

    mutable struct:public Object
    {
        char reserved[sizeof(T)];
        Node* next;
        Node* pre;
    }m_head;
    int m_length;
    int m_step;
    Node* m_current;

    Node* position(int i) const
    {
        Node* current = reinterpret_cast<Node*> (&m_head);
        for (int p = 0; p < i; ++p)
        {
            current = current->next;
        }
        return current;
    }

    virtual Node* create()
    {
        return new Node();
    }

    virtual void destroy(Node* t)
    {
        delete t;
    }

public:
    DualLinkList()
    {
        m_head.next = NULL;
        m_head.pre = NULL;
        m_length = 0;
        m_step = 0;
        m_current = NULL;
    }

    bool insert(int i, const T& t)
    {
        bool ret = ((0 <= i) && (i <= m_length));

        if (ret)
        {
            Node* node =  create();

            if(node)
            {
                Node* current = position(i);
                Node* next = current->next;

                node->value = t;
                node->next = next;
                current->next = node;

                if (current != reinterpret_cast<Node*> (&m_head))
                {
                    node->pre = current;
                }
                else
                {
                    node->pre = NULL;
                }

                if (next)
                {
                    next->pre = node;
                }

                m_length++;
            }
            else
            {
                EXCEPTION_THROW(NoEnoughMemoryException,"no enough memory to insert node...");
            }
        }
        else
        {
            EXCEPTION_THROW(IndexOutOfBoundsException, "invalid argument i...");
        }
        return ret;
    }

    bool insert(const T& t)
    {
        return insert(m_length,t);
    }


    bool remove(int i)
    {
        bool ret = ((0 <= i) && (i < m_length));

        if(ret)
        {
            Node* current = position(i);

            Node* todel = current->next;

            Node* next = todel->next;

            if (todel)
            {
                if (m_current == todel)
                {
                    m_current = m_current->next;
                }

                current->next = next;

                if (next)
                {
                    next->pre = current;
                }
                m_length--;

                destroy(todel);
            }
        }
        else
        {
            EXCEPTION_THROW(IndexOutOfBoundsException,"invalid argument i...");
        }
        return ret;
    }

    void clear()
    {
        while(m_head.next)
        {
            remove(0);
        }
    }

    bool get(int i, T &t) const
    {
        bool ret = ((0 <= i) && (i < m_length));
        if(ret)
        {
            t = (position(i))->next->value;
        }
        else
        {
            EXCEPTION_THROW(IndexOutOfBoundsException,"invalid argument i...");
        }
        return ret;
    }

    virtual T get(int i) const
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

    virtual int find(const T& t) const
    {
        int ret = -1;
        Node* node = m_head.next;

        for (int i = 0; i < m_length; i++, node = node->next)
        {
            if (node->value == t)
            {
                ret = i;
                break;
            }
        }
        return ret;
    }

    virtual bool move(int i, int step = 1)
    {
        bool ret = ((0 <= i) && (i < m_length) && (step > 0));

        if (ret)
        {
            m_current = position(i)->next;
            m_step = step;
            return ret;
        }
        else
        {
            EXCEPTION_THROW(IndexOutOfBoundsException,"invalid argument i ...");
        }
    }

    virtual bool end() const
    {
        return m_current==NULL;
    }

    virtual bool next()
    {
        int i;
        for (i = 0; i < m_step && !end(); ++i)
        {
            m_current = m_current->next;
        }
        return i == m_step;
    }

    virtual bool pre()
    {
        int i;
        for (i = 0; i < m_step && !end(); ++i)
        {
            m_current = m_current->pre;
        }
        return i == m_step;
    }

    virtual T current()
    {
        if (!end())
            return m_current->value;
        else
            EXCEPTION_THROW(InvalidOperationException,"no value at current position...");
    }

    bool set(int i, const T &t)
    {
        bool ret = ((0 <= i) && (i < m_length));
        if(ret)
        {
            (position(i))->next->value = t;
        }
        else
        {
            EXCEPTION_THROW(IndexOutOfBoundsException,"invalid argument i...");
        }
        return ret;
    }

    ~DualLinkList()
    {
        clear();
    }
};

}

#endif // DUALLINKLIST_H
