#ifndef SHAREDPOINTER_H
#define SHAREDPOINTER_H

#include "pointer.h"
#include "exception.h"
#include <cstdlib>

namespace HBCLib
{

template <typename T>
class SharedPointer: public Pointer<T>
{
protected:
    int* m_ref;

    void assign(const SharedPointer<T>& s)
    {
        this->m_pointer = s.m_pointer;
        this->m_ref = s.m_ref;

        if (m_ref)
        {
            (*m_ref)++;
        }
    }

public:
    SharedPointer(T* t = NULL): m_ref(NULL)
    {
        if (t)
        {
            //m_ref = static_cast<int*> (malloc(sizeof(int)));
            m_ref = new int;

            if (m_ref)
            {
                this->m_pointer = t;
                *m_ref = 1;
            }
            else
            {
                EXCEPTION_THROW(NoEnoughMemoryException,"no enough memory to create a sharedpointer object...");
            }
        }
    }

    SharedPointer(const SharedPointer<T>& s): Pointer<T>(NULL)
    {
        assign(s);
    }

    SharedPointer<T>& operator=(const SharedPointer<T>& s)
    {
        if (this != &s)
        {
            clear();

            assign(s);
        }

        return *this;
    }

    void clear()
    {
        T* temp = this->m_pointer;
        int* ref = m_ref;

        this->m_pointer = NULL;
        m_ref = NULL;

        if (ref)
        {
            (*ref)--;

            if (*ref == 0)
            {
                delete temp;
                //free(ref);
                delete ref;
            }
        }
    }

    ~SharedPointer()
    {
        clear();
    }
};

template <typename T>
bool operator==(const SharedPointer<T>& a, const SharedPointer<T>& b)
{
    return (a.get() == b.get());
}

template <typename T>
bool operator!=(const SharedPointer<T>& a, const SharedPointer<T>& b)
{
    return !(a == b);
}

}

#endif // SHAREDPOINTER_H
