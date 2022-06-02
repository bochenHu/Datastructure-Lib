#ifndef DUALCIRCLELINKLIST_H
#define DUALCIRCLELINKLIST_H

#include "DualLinkList.h"
#include <iostream>
using namespace std;

namespace HBCLib
{

template <typename T>
class DualCircleLinkList: public DualLinkList<T>
{
protected:
    typedef typename DualLinkList<T>::Node Node;
    Node* last()
    {
        return this->position(this->m_length - 1)->next;
    }

    Node* begin()
    {
        return this->m_head.next;
    }

    void last_to_begin()
    {
        last()->next = begin();
    }

    void begin_to_end()
    {
        begin()->pre = last();
    }

    int mod(int size) const
    {
        return (size == 0)?0:(size % (this->m_length));
    }

public:
    bool insert(int i, const T& t)
    {
        bool ret = ((0 <= i) && (i <= this->m_length));

        i = i % (this->m_length + 1);

        if (ret)
        {
            Node* node =  this->create();

            if(node)
            {
                if (i < this->m_length && i > 0 )
                {
                    Node* current = this->position(i);
                    Node* next = current->next;
                    node->value = t;
                    node->next = next;
                    current->next = node;

                    if (current != reinterpret_cast<Node*> (&this->m_head))
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

                    this->m_length++;
                }
                else if (i == 0)
                {
                    Node* current = reinterpret_cast<Node*> (& this->m_head);
                    Node* next = current->next;
                    if (this->m_length == 0)
                    {
                        node->value = t;

                        node->next = node;
                        current->next = node;

                        node->pre = node;
                    }
                    else if (this->m_length > 0)
                    {
                        node->value = t;
                        node->next = next;
                        current->next = node;

                        node->pre = next->pre;
                        next->pre = node;
                    }
                    this->m_length++;
                }
                else if (i == this->m_length)
                {
                    Node* current = begin()->pre;
                    Node* next = current->next;

                    node->value = t;
                    node->next = next;
                    current->next = node;

                    if (current != reinterpret_cast<Node*> (&this->m_head))
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

                    this->m_length++;
                }
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


        /*//bool ret = true;

        //i = i % (this->m_length + 1);

        //ret = DualLinkList<T>::insert(i,t);

        if (ret && i == 0 && this->m_length == 0)
        {
            begin()->next = begin();
            begin()->pre = begin();
        }
        else if (ret && i == 0 && this->m_length > 0)
        {
            begin()->next =
            begin()->pre = begin()->next->pre;

        }

        if (ret && i == this->m_length && i > 0)
        {
            last_to_begin();
            begin_to_end();
        }

        return ret;*/
    }

    bool insert(const T& t)
    {
        return insert(this->m_length,t);
    }

    bool remove(int i)
    {
        bool ret = true;

        i = mod(i);

        if (i == 0)
        {
            Node* todel = begin();
            if (todel)
            {
                this->m_head.next = todel->next;
                this->m_length--;

                if (this->m_length == 0)
                {
                    this->m_current = NULL;
                    this->m_head.next = NULL;
                    this->m_head.pre = NULL;
                }
                else
                {
                    last_to_begin();
                    begin_to_end();
                    if (this->m_current == todel)
                    {
                        this->m_current = todel->next;
                    }
                }
                this->destroy(todel);
            }
            else
            {
                ret = false;
            }
        }
        else if (i == this->m_length-1 && i > 0)
        {
            Node* todel = last();

            begin()->pre = todel->pre;
            todel->pre->next = begin();
            this->m_length--;

            if (this->m_current == todel)
            {
                this->m_current = todel->next;
            }
            this->destroy(todel);
        }
        else
        {
            ret = DualLinkList<T>::remove(i);
        }

        return ret;
    }

    bool remove_tail()
    {
        bool ret = true;

        Node* next = begin()->next;
        Node* last = begin()->pre;

        if (begin())
        {
            if (next)
            {
                begin()->pre = last->pre;
                last->pre->next = begin();

                if (last == this->m_current)
                {
                    this->m_current = begin();
                }
            }
            else
            {
                this->m_head.next = NULL;
                this->m_head.pre = NULL;
                this->m_current = NULL;
            }

            this->m_length--;
            this->destroy(last);
        }
        else
        {
            ret = false;
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

            this->m_head.pre = NULL;
            this->m_head.next = NULL;
            this->m_current = NULL;
            this->m_length = 0;

            destroy(todel);
        }
    }

    bool get(int i,const T& t) const
    {
        return DualLinkList<T>::get(mod(i),t);
    }

    T get(int i) const
    {
        return DualLinkList<T>::get(mod(i));
    }

    bool set(int i, const T& t)
    {
        return DualLinkList<T>::set(mod(i),t);
    }

    bool move(int i, int step = 1)
    {
        return DualLinkList<T>::move(mod(i),step);
    }

    bool end() const
    {
        return (this->m_current == NULL) || (this->m_length == 0);
    }

    ~DualCircleLinkList()
    {
        clear();
    }
};

}

#endif // DUALCIRCLELINKLIST_H
