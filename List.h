#ifndef LIST_H
#define LIST_H
#include "object.h"

namespace HBCLib
{

template <typename T>
class List : public Object
{
protected:
    List(const List<T>& t);
    List<T>& operator=(const List<T>& t);
public:
    List() {}
    virtual bool insert(const T& t) = 0;
    virtual bool insert(int i, const T& t) = 0;

    virtual bool remove(int i) = 0;
    virtual void clear() = 0;

    virtual bool get(int i, T& t) const = 0;
    virtual T get(int i) const = 0;
    virtual int length() const = 0;
    virtual int find(const T& t) const = 0;

    virtual bool set(int i, const T& t) = 0;
};

}

#endif // LIST_H
