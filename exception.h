#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <iostream>
#include "object.h"
using namespace std;
namespace HBCLib
{

#define EXCEPTION_THROW(a,x) (throw a(x,__FILE__,__LINE__))

class Exception : public Object
{
protected:
    char* m_message;
    char* m_location;

    void init(const char* message, const char* File, int Line);
public:
    Exception(const char* message);
    Exception(const char* File, int Line);
    Exception(const char* message, const char* File, int Line);

    Exception(const Exception& ex);
    Exception& operator=(const Exception& ex);

    virtual const char* message()const;
    virtual const char* location()const;

    virtual ~Exception() = 0;
};

class ArithmeticException : public Exception
{
public:
    ArithmeticException(): Exception(0){}
    ArithmeticException(const char* message): Exception(message){}
    ArithmeticException(const char* File, int Line): Exception(File, Line){}
    ArithmeticException(const char* message, const char* File, int Line)
        : Exception(message,File,Line){}

    ArithmeticException(const ArithmeticException& aex) : Exception(aex){}
    ArithmeticException& operator=(const ArithmeticException& aex)
    {
        Exception::operator =(aex);

        return *this;
    }
    virtual const char* message() const
    {
        cout<<"Arithmetic exception "<<endl;
        return Exception::message();
    }
};

class IndexOutOfBoundsException : public Exception
{
public:
    IndexOutOfBoundsException(): Exception(0){}
    IndexOutOfBoundsException(const char* message): Exception(message){}
    IndexOutOfBoundsException(const char* File, int Line): Exception(File, Line){}
    IndexOutOfBoundsException(const char* message, const char* File, int Line)
        : Exception(message,File,Line){}

    IndexOutOfBoundsException(const IndexOutOfBoundsException& aex) : Exception(aex){}
    IndexOutOfBoundsException& operator=(const IndexOutOfBoundsException& aex)
    {
        Exception::operator =(aex);

        return *this;
    }
    virtual const char* message() const
    {
        cout<<"IndexOutOfBounds exception "<<endl;
        return Exception::message();
    }
};

class NoEnoughMemoryException : public Exception
{
public:
    NoEnoughMemoryException(): Exception(0){}
    NoEnoughMemoryException(const char* message): Exception(message){}
    NoEnoughMemoryException(const char* File, int Line): Exception(File, Line){}
    NoEnoughMemoryException(const char* message, const char* File, int Line)
        : Exception(message,File,Line){}

    NoEnoughMemoryException(const NoEnoughMemoryException& aex) : Exception(aex){}
    NoEnoughMemoryException& operator=(const NoEnoughMemoryException& aex)
    {
        Exception::operator =(aex);

        return *this;
    }
    virtual const char* message() const
    {
        cout<<"NoEnoughMemory exception "<<endl;
        return Exception::message();
    }
};

class InvalidParameterException : public Exception
{
public:
    InvalidParameterException(): Exception(0){}
    InvalidParameterException(const char* message): Exception(message){}
    InvalidParameterException(const char* File, int Line): Exception(File, Line){}
    InvalidParameterException(const char* message, const char* File, int Line)
        : Exception(message,File,Line){}

    InvalidParameterException(const InvalidParameterException& aex) : Exception(aex){}
    InvalidParameterException& operator=(const InvalidParameterException& aex)
    {
        Exception::operator =(aex);

        return *this;
    }
    virtual const char* message() const
    {
        cout<<"InvalidParameter exception "<<endl;
        return Exception::message();
    }
};


class NullPointerException : public Exception
{
public:
    NullPointerException(): Exception(0){}
    NullPointerException(const char* message): Exception(message){}
    NullPointerException(const char* File, int Line): Exception(File, Line){}
    NullPointerException(const char* message, const char* File, int Line)
        : Exception(message,File,Line){}

    NullPointerException(const NullPointerException& aex) : Exception(aex){}
    NullPointerException& operator=(const NullPointerException& aex)
    {
        Exception::operator =(aex);

        return *this;
    }
    virtual const char* message() const
    {
        cout<<"InvalidParameter exception "<<endl;
        return Exception::message();
    }
};

class InvalidOperationException : public Exception
{
public:
    InvalidOperationException(): Exception(0){}
    InvalidOperationException(const char* message): Exception(message){}
    InvalidOperationException(const char* File, int Line): Exception(File, Line){}
    InvalidOperationException(const char* message, const char* File, int Line)
        : Exception(message,File,Line){}

    InvalidOperationException(const InvalidOperationException& aex) : Exception(aex){}
    InvalidOperationException& operator=(const InvalidOperationException& aex)
    {
        Exception::operator =(aex);

        return *this;
    }
    virtual const char* message() const
    {
        cout<<"InvalidParameter exception "<<endl;
        return Exception::message();
    }
};

}


#endif // EXCEPTION_H
