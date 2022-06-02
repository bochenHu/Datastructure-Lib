#ifndef BTREE_H
#define BTREE_H

#include "Tree.h"
#include "BTreeNode.h"
#include "exception.h"
#include "LinkQueue.h"
#include "DynamicArray.h"

namespace HBCLib
{

enum BTTraversal
{
    Pre,
    In,
    Post,
    Level
};

template <typename T>
class BTree: public Tree<T>
{
protected:
    LinkQueue<BTreeNode<T>*> m_queue;

    BTreeNode<T>* find(BTreeNode<T>* Node, const T& value) const
    {
        BTreeNode<T>* ret = NULL;

        if (Node != NULL)
        {
            if (Node->value == value)
            {
                ret = Node;
            }
            else
            {
                if (ret == NULL)
                {
                    ret = find(Node->left,value);
                }

                if (ret == NULL)
                {
                    ret = find(Node->right,value);
                }
            }
        }

        return ret;
    }

    BTreeNode<T>* find(BTreeNode<T>* Node, BTreeNode<T>* obj) const
    {
        BTreeNode<T>* ret = NULL;

        if (Node == obj)
        {
            ret = Node;
        }
        else
        {
            if (Node != NULL)
            {
                if (ret == NULL)
                {
                    ret = find(Node->left,obj);
                }

                if (ret == NULL)
                {
                    ret = find(Node->right,obj);
                }
            }
        }

        return ret;
    }

    bool insert(BTreeNode<T>* Node,BTreeNode<T>* parent, BTNodePos pos)
    {
        bool ret = true;

        if (pos == ANY)
        {
            if (parent->left == NULL)
            {
                parent->left = Node;
            }
            else if (parent->right == NULL)
            {
                parent->right = Node;
            }
            else
            {
                ret = false;
            }
        }
        else if (pos == LEFT)
        {
            if (parent->left == NULL)
            {
                parent->left = Node;
            }
            else
            {
                ret = false;
            }
        }
        else if (pos == RIGHT)
        {
            if (parent->right == NULL)
            {
                parent->right = Node;
            }
            else
            {
                ret = false;
            }
        }
        else
        {
            ret = false;
        }

        return ret;
    }

    void free(BTreeNode<T>* Node)
    {
        if (Node)
        {
            if (Node->left)
            {
                free(Node->left);
            }

            if (Node->right)
            {
                free(Node->right);
            }

            if (Node->flag())
            {
                delete Node;
            }
        }
    }

    void remove(BTreeNode<T>* Node, BTree<T>*& bt)
    {
        bt = new BTree<T>();

        if (bt == NULL)
        {
            EXCEPTION_THROW(NoEnoughMemoryException,"no enough memory to create a new btree...");
        }
        else
        {
            if (Node == root())
            {
                this->m_root = NULL;
            }
            else
            {
                BTreeNode<T>* np = dynamic_cast<BTreeNode<T>*>(Node->parent);
                if (np->left == Node)
                {
                    np->left = NULL;
                }
                else if (np->right == Node)
                {
                    np->right = NULL;
                }

                Node->parent = NULL;
            }

            bt->m_root = Node;
        }
    }

    int count(BTreeNode<T>* Node)const
    {
        return ((Node != NULL) ? (count(Node->left)+count(Node->right)+1) : (0));
    }

    int height(BTreeNode<T>* Node)const
    {
        int h = 0;

        if (Node != NULL)
        {
            int hl = height(Node->left);
            int hr = height(Node->right);

            h = ((hl>hr) ? (hl) : (hr)) + 1;
        }

        return h;
    }

    int degree(BTreeNode<T>* Node)const
    {
        int d = 0;

        if (Node != NULL)
        {
            d = (!!Node->left + !!Node->right);

            if (Node->left && (d < 2))
            {
                int dl = degree(Node->left);

                if (dl > d)
                {
                    d = dl;
                }
            }

            if (Node->right && (d < 2))
            {
                int dr = degree(Node->right);

                if (dr > d)
                {
                    d = dr;
                }
            }
        }

        return d;
    }

    void Pre_traversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue)
    {
        if (node)
        {
            queue.enqueue(node);
            Pre_traversal(node->left,queue);
            Pre_traversal(node->right,queue);
        }
    }

    void In_traversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue)
    {
        if (node)
        {
            In_traversal(node->left,queue);
            queue.enqueue(node);
            In_traversal(node->right,queue);
        }
    }

    void Post_traversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue)
    {
        if (node)
        {
            Post_traversal(node->left,queue);
            Post_traversal(node->right,queue);
            queue.enqueue(node);
        }
    }

    void Level_traversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue)
    {
        if (node)
        {
            LinkQueue<BTreeNode<T>*> lq;

            lq.enqueue(node);

            while(lq.length() > 0)
            {
                BTreeNode<T>* n = lq.front();
                if (n->left)
                {
                    lq.enqueue(n->left);
                }

                if (n->right)
                {
                    lq.enqueue(n->right);
                }

                queue.enqueue(n);
                lq.dequeue();
            }
        }
    }

    BTreeNode<T>* clone(const BTreeNode<T>* node) const
    {
        BTreeNode<T>* ret = NULL;

        if (node != NULL)
        {
            ret = BTreeNode<T>::NewNode();

            if (ret)
            {
                ret->value = node->value;
                ret->left = clone(node->left);
                ret->right = clone(node->right);

                if (ret->left)
                {
                    ret->left->parent = ret;
                }

                if (ret->right)
                {
                    ret->right->parent = ret;
                }
            }
            else
            {
                EXCEPTION_THROW(NoEnoughMemoryException,"no enough memory to create a new btreenode...");
            }
        }

        return ret;
    }

    bool equal(const BTreeNode<T>* na,const BTreeNode<T>* nb) const
    {
        bool ret = true;

        if ((na && !nb) || (!na && nb))
        {
            ret = false;
        }
        else if (na && nb)
        {
            ret = (na->value == nb->value) && equal(na->left, nb->left) && equal(na->right,nb->right);
        }

        return ret;
    }

    BTreeNode<T>* add(const BTreeNode<T>* na,const BTreeNode<T>* nb) const
    {
        BTreeNode<T>* ret = NULL;

        if (na && nb)
        {
            ret = BTreeNode<T>::NewNode();

            if (ret)
            {
                ret->value = na->value + nb->value;
                ret->left = add(na->left,nb->left);
                ret->right = add(na->right, nb->right);

                if (ret->left)
                {
                    ret->left->parent = ret;
                }

                if (ret->right)
                {
                    ret->right->parent = ret;
                }
            }
            else
            {
                EXCEPTION_THROW(NoEnoughMemoryException,"no enough memory to create a new btreenode...");
            }

        }
        else if (!na && nb)
        {
            ret = clone(nb);
        }
        else if (na && !nb)
        {
            ret = clone(na);
        }

        return ret;
    }

    void traversal(BTTraversal order, LinkQueue<BTreeNode<T>*>& queue)
    {
        switch(order)
        {
        case Pre: Pre_traversal(root(),queue);
            break;
        case In:  In_traversal(root(),queue);
            break;
        case Post:  Post_traversal(root(),queue);
            break;
        case Level:  Level_traversal(root(),queue);
            break;
        default:
            EXCEPTION_THROW(InvalidParameterException,"parameter order is invalid...");
            break;
        }
    }

    BTreeNode<T>* connect(LinkQueue<BTreeNode<T>*>& queue)
    {
        BTreeNode<T>* ret = queue.front();

        if (queue.length() > 0)
        {
            BTreeNode<T>* slider = queue.front();

            queue.dequeue();
            slider->left = NULL;

            while (queue.length() > 0)
            {
                slider->right = queue.front();
                queue.front()->left = slider;
                slider = queue.front();
                queue.dequeue();
            }

            slider->right = NULL;
        }

        return ret;
    }

public:
    bool insert(TreeNode<T>* Node)
    {
        return insert(Node,ANY);
    }

    bool insert(TreeNode<T>* Node, BTNodePos pos)
    {
        bool ret = true;

        if (Node != NULL)
        {
            if (this->m_root == NULL)
            {
                Node->parent = NULL;
                this->m_root = Node;
            }
            else
            {
                BTreeNode<T>* par = find(Node->parent);

                if (par)
                {
                    ret = insert(dynamic_cast<BTreeNode<T>*>(Node),par,pos);
                }
                else
                {
                    EXCEPTION_THROW(InvalidOperationException,"cannot find Node parent Node...");
                }
            }
        }
        else
        {
            EXCEPTION_THROW(InvalidParameterException,"Tree Node cannot be NULL...");
        }

        return ret;
    }

    bool insert(const T& t, TreeNode<T>* parent)
    {
        return insert(t,parent,ANY);
    }

    bool insert(const T& t, TreeNode<T>* parent, BTNodePos pos)
    {
        bool ret = true;
        BTreeNode<T>* Node = BTreeNode<T>::NewNode();

        if (Node)
        {
            Node->value = t;
            Node->parent = parent;
            ret = insert(Node,pos);

            if (!ret)
            {
                delete Node;
            }
        }
        else
        {
            EXCEPTION_THROW(NoEnoughMemoryException, "no enough memory to create a BTreeNode...");
        }

        return ret;
    }

    SharedPointer< Tree<T> > remove(const T& t)
    {
        BTree<T>* ret = NULL;

        TreeNode<T>* Node = find(t);

        if (Node)
        {
            remove(dynamic_cast<BTreeNode<T>*>(Node),ret);
            m_queue.clear();
        }
        else
        {
            EXCEPTION_THROW(InvalidParameterException,"parameter t is invalid...");
        }

        return ret;
    }

    SharedPointer< Tree<T> > remove(TreeNode<T>* Node)
    {
        BTree<T>* ret = NULL;

        Node = find(Node);

        if (Node)
        {
            remove(dynamic_cast<BTreeNode<T>*>(Node),ret);
            m_queue.clear();
        }
        else
        {
            EXCEPTION_THROW(InvalidParameterException,"parameter Node is invalid...");
        }

        return ret;
    }

    void clear()
    {
        free(root());

        this->m_root = NULL;

        m_queue.clear();
    }

    BTreeNode<T>* find(const T& t) const
    {
        return find(root(),t);
    }

    BTreeNode<T>* find(TreeNode<T>* Node) const
    {
        return find(root(),dynamic_cast<BTreeNode<T>*> (Node));
    }

    BTreeNode<T>* root() const
    {
        return dynamic_cast<BTreeNode<T>*>(this->m_root);
    }
    int degree() const
    {
        return degree(root());
    }

    int height() const
    {
        return height(root());
    }

    int count() const
    {
        return count(root());
    }

    bool begin()
    {
        bool ret = (root()!= NULL);

        if (ret)
        {
            m_queue.clear();
            m_queue.enqueue(root());
        }

        return ret;
    }

    bool end()
    {
        return m_queue.length() == 0;
    }

    T current()
    {
        if (!end())
        {
            return m_queue.front()->value;
        }
        else
        {
            EXCEPTION_THROW(InvalidOperationException,"m_queue is NULL...");
        }
    }

    bool next()
    {
        bool ret = (m_queue.length() != 0);

        if (ret)
        {
            BTreeNode<T>* Node = m_queue.front();

            if (Node->left)
            {
                m_queue.enqueue(Node->left);
            }

            if (Node->right)
            {
                m_queue.enqueue(Node->right);
            }

            m_queue.dequeue();
        }

        return ret;
    }

    SharedPointer< Array<T> > traversal(BTTraversal order)
    {
        DynamicArray<T>* ret = NULL;
        LinkQueue<BTreeNode<T>*> queue;

        traversal(order,queue);

        ret = new DynamicArray<T>(queue.length());

        for (int i = 0; i < ret->length(); i++, queue.dequeue())
        {
            ret->set(i, queue.front()->value);
        }

        return ret;
    }

    SharedPointer< BTree<T> > clone() const
    {
        BTree<T>* ret = new BTree<T>();

        if (ret)
        {
            ret->m_root = clone(root());
        }
        else
        {
            EXCEPTION_THROW(NoEnoughMemoryException,"no enough memory to create a new btree...");
        }

        return ret;
    }

    bool operator==(const BTree<T>& bt) const
    {
        return equal(root(), bt.root());
    }

    bool operator!=(const BTree<T>& bt) const
    {
        return !((*this)==(bt));
    }

    SharedPointer< BTree<T> > add(const BTree<T>& bt) const
    {
        BTree<T>* ret = new BTree<T>();

        if (ret)
        {
            ret->m_root =  add(root(), bt.root());
        }
        else
        {
            EXCEPTION_THROW(NoEnoughMemoryException,"no enough memory to create a new btree...");
        }

        return ret;
    }

    BTreeNode<T>* thread(BTTraversal order)
    {
        BTreeNode<T>* ret = NULL;
        LinkQueue<BTreeNode<T>*> queue;

        traversal(order,queue);

        ret = connect(queue);

        this->m_root = NULL;

        m_queue.clear();

        return ret;
    }

    ~BTree()
    {
        clear();
    }
};

}

#endif // BTREE_H
