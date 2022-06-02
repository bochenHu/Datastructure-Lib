#ifndef CIRCLELIST_H
#define CIRCLELIST_H

#include "LinkList.h"

namespace HBCLib
{

template <typename T>
class CircleList:public LinkList<T>
{
protected:
    typedef typename LinkList<T>::Node Node;

    Node* last() const
    {
        return LinkList<T>::position(this->m_length-1)->next;
    }

    void last_to_first()
    {
        last()->next = this->m_head.next;
    }

    int mod(int i) const
    {
        return this->m_length == 0? 0 : (i % (this->m_length));
    }
public:
    bool insert(int i, const T& t)
    {
        bool ret = true;

        int x = i % (this->m_length+1);

        ret = LinkList<T>::insert(x,t);

        if (ret && (x == 0))
        {
            last_to_first();
        }
        return ret;
    }

    bool insert(const T& t)
    {
        return insert(this->m_length,t);
    }

    bool remove(int i)
    {
        bool ret = true;

        int x = mod(i);

        if (x == 0)
        {
            Node* todel = this->m_head.next;
            if (todel)
            {
                this->m_head.next = todel->next;
                this->m_length--;

                if (this->m_length > 0)
                {
                    last_to_first();

                    if (this->m_current == todel)
                    {
                        this->m_current = this->m_current->next;
                    }
                }
                else
                {
                    this->m_head.next = NULL;
                    this->m_current = NULL;
                }
                this->destroy(todel);
            }
            else
            {
                ret = false;
            }
        }
        else
        {
            LinkList<T>::remove(i);
        }
        return ret;
    }

    void clear()
    {
        while(this->m_length > 1)
        {
            remove(1);
        }

        if (this->m_length == 1)
        {
            Node* todel = this->m_head.next;

            this->m_head.next = NULL;
            this->m_current = NULL;
            this->m_length = 0;

            this->destroy(todel);
        }
    }

    int find(const T& t) const
    {
        int ret = -1;

        Node* temp = this->m_head.next;

        for (int i = 0; i < this->m_length; ++i,temp = temp->next)
        {
            if (temp->value == t)
            {
                ret = i;
                break;
            }
        }
        return ret;
    }

    T get(int i) const
    {
        return LinkList<T>::get(mod(i));
    }

    bool get(int i,T& t) const
    {
        return LinkList<T>::get(mod(i),t);
    }

    bool set(int i, const T& t)
    {
        return LinkList<T>::set(mod(i),t);
    }

    bool move(int i, int step = 1)
    {
        return LinkList<T>::move(mod(i),step);
    }

    bool end() const
    {
        return (this->m_length == 0) || (this->m_current == NULL);
    }

    ~CircleList()
    {
        clear();
    }
};
}

#endif // CIRCLELIST_H
