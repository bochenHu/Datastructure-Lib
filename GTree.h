#ifndef GTREE_H
#define GTREE_H

#include "GTreeNode.h"
#include "Tree.h"
#include "exception.h"
#include "LinkQueue.h"

namespace HBCLib
{

template <typename T>
class GTree: public Tree<T>
{
protected:
    LinkQueue<GTreeNode<T>*> m_queue;

    GTreeNode<T>* find(GTreeNode<T>* root, const T& t) const
    {
        GTreeNode<T>* temp = NULL;

        if (root != NULL)
        {
            if (root->value == t)
            {
                return root;
            }
            else
            {
                if ((root->child.length() != 0))
                {
                    for (root->child.move(0); !root->child.end() && (temp == NULL); root->child.next())
                    {
                        temp = find(root->child.current(),t);
                    }
                }
            }
        }
        return temp;
    }

    GTreeNode<T>* find(GTreeNode<T>* root, GTreeNode<T>* obj) const
    {
        GTreeNode<T>* ret = NULL;

        if (root == obj)
        {
            ret = root;
        }
        else
        {
            if ((root != NULL) && (root->child.length() != 0))
            {
                for (root->child.move(0);!(root->child.end()) && (ret == NULL); root->child.next())
                {
                    ret = find(root->child.current(),obj);
                }
            }
        }

        return ret;
    }

    void free(GTreeNode<T>* root)
    {
        if (root != NULL)
        {
            if (root->child.length() != 0)
            {
                for (root->child.move(0);!root->child.end();root->child.next())
                {
                    free(root->child.current());
                }
            }

            if (root->flag())
            {
                delete root;
            }
        }
    }

    void remove(GTreeNode<T>* Node, GTree<T>*& ret)
    {
        ret = new GTree();

        if (ret == NULL)
        {
            EXCEPTION_THROW(NoEnoughMemoryException,"no enough memory to return a new GTree...");
        }
        else
        {
            if (root() == Node)
            {
                this->m_root = NULL;
            }
            else
            {
                LinkList<GTreeNode<T>*>& child = dynamic_cast<GTreeNode<T>*>(Node->parent)->child;

                child.remove(child.find(Node));

                Node->parent = NULL;
            }

            ret->m_root = Node;
        }
    }

    int count(GTreeNode<T>* Node) const
    {
        int ret = 0;

        if (Node != NULL)
        {
            ret = 1;

            if (Node->child.length() != 0)
            {
                for (Node->child.move(0); !Node->child.end(); Node->child.next())
                {
                    ret += count(Node->child.current());
                }
            }
        }

        return ret;
    }

    int degree(GTreeNode<T>* Node) const
    {
        int ret = 0;

        if (Node != NULL)
        {
            if (Node->child.length() != 0)
            {
                ret = Node->child.length();

                for (Node->child.move(0); !Node->child.end(); Node->child.next())
                {
                    if (degree(Node->child.current()) > ret)
                    {
                        ret = degree(Node->child.current());
                    }
                }
            }
        }

        return ret;
    }

    int height(GTreeNode<T>* Node) const
    {
        int ret = 0;

        if (Node != NULL)
        {
            ret = 1;

            if (Node->child.length() != 0)
            {
                int dep = 0;

                for (Node->child.move(0); !Node->child.end(); Node->child.next())
                {
                    if (height(Node->child.current()) > dep)
                    {
                        dep = height(Node->child.current());
                    }
                }

                ret += dep;
            }
        }

        return ret;
    }

public:
    GTree()
    {

    }

    bool insert(TreeNode<T>* Node)
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
                GTreeNode<T>* temp = find(Node->parent);

                if (temp != NULL)
                {
                    GTreeNode<T>* n = dynamic_cast<GTreeNode<T>*> (Node);

                    if (temp->child.find(n) < 0)
                    {
                        temp->child.insert(n);
                    }
                }
                else
                {
                    EXCEPTION_THROW(InvalidOperationException,"Node has been in the tree...");
                }
            }
        }
        else
        {
            EXCEPTION_THROW(InvalidParameterException,"Parameter Node cannot be NULL...");
        }

        return ret;
    }

    bool insert(const T& t, TreeNode<T>* parent)
    {
        bool ret = true;
        GTreeNode<T>* Node = GTreeNode<T>::NewNode();

        if (Node != NULL)
        {
            Node->parent = parent;
            Node->value = t;

            insert(Node);
        }
        else
        {
            EXCEPTION_THROW(NoEnoughMemoryException,"no memory to create a tree node...");
        }

        return ret;
    }

    SharedPointer< Tree<T> > remove(const T& t)
    {
        GTree<T>* ret = NULL;

        GTreeNode<T>* temp = find(t);

        if (temp == NULL)
        {
            EXCEPTION_THROW(InvalidParameterException,"cannot find parameter t...");
        }
        else
        {
            remove(temp,ret);
            m_queue.clear();
        }

        return ret;
    }

    SharedPointer< Tree<T> > remove(TreeNode<T>* Node)
    {
        GTree<T>* ret = NULL;

        Node = find(Node);

        if (Node == NULL)
        {
            EXCEPTION_THROW(InvalidParameterException,"cannot find parameter Node...");
        }
        else
        {
            remove(dynamic_cast<GTreeNode<T>*>(Node),ret);
            m_queue.clear();
        }

        return ret;
    }

    void clear()
    {
        free(root());
        this->m_root = NULL;
        m_queue.clear();
    }

    GTreeNode<T>* find(const T& t) const
    {
        return find(root(),t);
    }

    GTreeNode<T>* find(TreeNode<T>* Node) const
    {
        return find(root(),dynamic_cast<GTreeNode<T>*>(Node));
    }

    GTreeNode<T>* root() const
    {
        return dynamic_cast<GTreeNode<T>*> (this->m_root);
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
        bool ret = (root() != NULL);

        if (ret)
        {
            m_queue.clear();
            m_queue.enqueue(root());
        }

        return ret;
    }

    bool end()
    {
        return (m_queue.length() == 0);
    }

    bool next()
    {
        bool ret = (m_queue.length() > 0);

        if (ret)
        {
            GTreeNode<T>* Node = m_queue.front();

            if (Node->child.length() != 0)
            {
                for (Node->child.move(0);!Node->child.end();Node->child.next())
                {
                    m_queue.enqueue(Node->child.current());
                }
            }

            m_queue.dequeue();
        }

        return ret;
    }

    T current()
    {
        if (! end())
        {
            return m_queue.front()->value;
        }
        else
        {
            EXCEPTION_THROW(InvalidOperationException,"no value at current position...");
        }
    }

    ~GTree()
    {
        clear();
    }
};

}

#endif // GTREE_H
