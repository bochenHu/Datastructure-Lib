#ifndef STACK_H
#define STACK_H

#include "object.h"

namespace HBCLib
{

template <typename T>
class Stack: public Object
{
    virtual void push(const T& t) = 0;
    virtual void pop() = 0;
    virtual void clear() = 0;
    virtual int size() const = 0;
    virtual T top() const = 0;
};

}

#endif // STACK_H
