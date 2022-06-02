#ifndef DYNAMICLIST_H
#define DYNAMICLIST_H

#include "SeqList.h"
#include "exception.h"

namespace HBCLib
{

template <typename T>
class DynamicList: public SeqList<T>
{
protected:
    int m_capacity;
public:
    DynamicList(int capacity)
    {
        SeqList<T>:: m_array = new T[capacity];

        if(this -> m_array != NULL)
        {
            this -> m_length = 0;
            m_capacity = capacity;
        }
        else
        {
            EXCEPTION_THROW(NoEnoughMemoryException,"no enough memory to create dynamiclist object...");
        }
    }

    int capacity() const
    {
        return m_capacity;
    }

    void resize(int capacity)
    {
        if(capacity != m_capacity)
        {
            T* t = new T[capacity];

            if (t != NULL)
            {
                int len = ((capacity < this -> m_length)? capacity:this -> m_length);

                for (int i = 0; i < len; ++i)
                {
                    t[i] = this -> m_array[i];
                }

                T* temp = this -> m_array;

                this -> m_length = len;
                this -> m_array = t;
                m_capacity = capacity;

                delete [] temp;
            }
            else
            {
                EXCEPTION_THROW(NoEnoughMemoryException,"no enough memory to resize dynamiclist object...");
            }
        }
    }

    ~DynamicList()
    {
        delete[] this -> m_array;
    }

};

}
#endif // DYNAMICLIST_H
