#ifndef TREENODE_H
#define TREENODE_H

#include "object.h"

namespace HBCLib
{

template <typename T>
class TreeNode: public Object
{
protected:
    TreeNode(const TreeNode<T>& gt);
    TreeNode<T>& operator=(const TreeNode<T>& gt);

    bool m_flag;

    void* operator new(unsigned int size)
    {
        return Object::operator new(size);
    }
public:
    T value;
    TreeNode<T>* parent;

    TreeNode()
    {
        m_flag = false;
        parent = NULL;
    }

    bool flag() const
    {
        return m_flag;
    }

    virtual ~TreeNode() = 0;
};

template <typename T>
TreeNode<T>::~TreeNode()
{
}

}

#endif // TREENODE_H
