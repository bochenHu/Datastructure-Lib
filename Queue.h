#ifndef QUEUE_H
#define QUEUE_H

#include "object.h"

namespace HBCLib
{

template <typename T>
class Queue: public Object
{
public:
    virtual void enqueue(const T& t) = 0;
    virtual void dequeue() = 0;
    virtual void clear() = 0;
    virtual int length() const = 0;
    virtual T front() const = 0;
};

}
#endif // QUEUE_H
