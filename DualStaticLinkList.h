#ifndef DUALSTATICLINKLIST_H
#define DUALSTATICLINKLIST_H

#include "DualLinkList.h"

namespace HBCLib
{

template <typename T, int N>
class DualStaticLinkList: public DualLinkList<T>
{
protected:
    typedef typename DualLinkList<T>::Node Node;

    struct SNode: public Node
    {
        void* operator new(unsigned int size, void* p)
        {
            return p;
        }
    };

    unsigned char m_space[sizeof(SNode) * N];
    int m_used[N];

    Node* create()
    {
        SNode* temp = NULL;

        for (int i = 0; i < N ; ++i)
        {
            if (m_used[i] == 0)
            {
                temp = new(reinterpret_cast<SNode*> (m_space) + i) SNode();
                m_used[i] = 1;
                break;
            }
        }

        return temp;
    }

    void destroy(Node *t)
    {
        SNode* temp = reinterpret_cast<SNode*> (m_space);
        SNode* st = dynamic_cast<SNode*> (t);

        for (int i = 0; i < N; ++i)
        {
            if (st == temp + i)
            {
                st->~SNode();
                m_used[i] = 0;
                break;
            }
        }
    }
public:
    DualStaticLinkList(): DualLinkList<T>()
    {
        for (int i = 0; i < N; ++i)
        {
            m_used[i] = 0;
        }
    }

    int capacity() const
    {
        return N;
    }

    ~DualStaticLinkList()
    {
        DualLinkList<T>::clear();
    }
};
}
#endif // DUALSTATICLINKLIST_H
