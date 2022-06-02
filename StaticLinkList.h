#ifndef STATICLINKLIST_H
#define STATICLINKLIST_H

#include "LinkList.h"

namespace HBCLib
{

template <typename T, int N>
class StaticLinkList: public LinkList<T>
{
protected:
    typedef typename LinkList<T>::Node Node;
    struct SNode: public Node
    {
        void* operator new(unsigned int size, void* p)
        {
            (void) size;
            return p;
        }
    };

    unsigned char m_space[sizeof(SNode) * N];
    int m_used[N];

    Node* create()
    {
        SNode* ret = NULL;  //reinterpret_cast<SNode*> (m_space);

        for (int i = 0; i < N ; ++i)
        {
            if (!m_used[i])
            {
                ret = reinterpret_cast<SNode*> (m_space) + i;
                ret = new(ret) SNode();
                m_used[i] = 1;
                break;
            }
        }
        return ret;
    }

    void destroy(Node *t)
    {
        SNode* sn = reinterpret_cast<SNode*> (m_space);
        SNode* x = dynamic_cast<SNode*> (t);

        for (int i = 0; i < N; i++)
        {
            if (x == sn + i)                         //OR x == sn + i
            {
                m_used[i] = 0;
                x->~SNode();
                break;
            }
        }
    }

public:
    StaticLinkList() : LinkList<T>()
    {
        for (int i = 0; i < N ; ++i)
        {
            m_used[i] = 0;
        }
    }

    int capacity() const
    {
        return N;
    }

    ~StaticLinkList()
    {
        LinkList<T>::clear();
    }

};
}

#endif // STATICLINKLIST_H
