#ifndef TREE_H
#define TREE_H

#include "TreeNode.h"
#include "SharedPointer.h"

namespace HBCLib
{

template <typename T>
class Tree: public Object
{
protected:
    TreeNode<T>* m_root;

    Tree(const Tree<T>& gt);
    Tree<T>& operator=(const Tree<T>& gt);
public:
    Tree() {m_root = NULL;}
    virtual bool insert(TreeNode<T>* Node) = 0;
    virtual bool insert(const T& t, TreeNode<T>* parent) = 0;
    virtual SharedPointer< Tree<T> > remove(const T& t) = 0;
    virtual SharedPointer< Tree<T> > remove(TreeNode<T>* Node) = 0;
    virtual void clear() = 0;
    virtual TreeNode<T>* find(const T& t) const = 0;
    virtual TreeNode<T>* find(TreeNode<T>* Node) const = 0;
    virtual TreeNode<T>* root() const = 0;
    virtual int degree() const = 0;
    virtual int height() const = 0;
    virtual int count() const = 0;
    virtual bool begin() = 0;
    virtual bool end() = 0;
    virtual bool next() = 0;
    virtual T current() = 0;
};

}

#endif // TREE_H
