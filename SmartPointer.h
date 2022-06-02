#ifndef SMARTPOINTER_H
#define SMARTPOINTER_H

#include "Pointer.h"

namespace HBCLib
{

template <typename T>
class SmartPointer : public Pointer<T>
{
public:
    SmartPointer(T* t = NULL) : Pointer<T>(t)
    {
    }

    SmartPointer(const SmartPointer<T>& smp)
    {
        this->m_pointer = smp.m_pointer;

        const_cast<SmartPointer<T>&> (smp).m_pointer = NULL;
    }

    SmartPointer<T>&
    operator=(const SmartPointer<T>& smp)
    {
        if(this != &smp)
        {
            T* temp = this->m_pointer;

            this->m_pointer = smp.m_pointer;
            const_cast<SmartPointer<T>&> (smp).m_pointer = NULL;
            delete temp;
        }
        return *this;
    }

    ~SmartPointer()
    {
        delete this->m_pointer;
    }
};

}

#endif // SMARTPOINTER_H
