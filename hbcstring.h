#ifndef HBCSTRING_H
#define HBCSTRING_H

#include "object.h"
#include "exception.h"

namespace HBCLib
{

class String : public Object
{
protected:
    char* m_str;
    int m_length;

    void init(const char* s);
    bool compare(const char* a,const char* b, int len) const;
    int* make_pmt(const char* p) const;
    int kmp(const char* s, const char* p) const;
public:
    String();
    String(const char* s);
    String(const String& t);
    String(const char c);

    int length() const;
    const char* str() const;

    char operator[](int i) const;
    char& operator[](int i);

    bool startWith(const char* s) const;
    bool startWith(const String& s) const;
    bool endOf(const char* s) const;
    bool endOf(const String& s) const;

    String& insert(int i, const char* s);
    String& insert(int i, const String& s);

    String& trim();

    int indexOf(const char* s) const;
    int indexOf(const String& s) const;

    String& remove(int i, int len);
    String& remove(const char* s);
    String& remove(const String& s);

    String& replace(const char* t, const char* s);
    String& replace(const char* t, const String s);
    String& replace(const String t, const char* s);
    String& replace(const String t, const String s);

    String sub(int i, int len) const;

    bool operator==(const char* s) const;
    bool operator==(const String& s) const;
    bool operator!=(const char* s) const;
    bool operator!=(const String& s) const;
    bool operator>(const char* s) const;
    bool operator>(const String& s) const;
    bool operator<(const char* s) const;
    bool operator<(const String& s) const;
    bool operator>=(const char* s) const;
    bool operator>=(const String& s) const;
    bool operator<=(const char* s) const;
    bool operator<=(const String& s) const;

    String operator+(const char* s) const;
    String operator+(const String& s) const;
    String& operator+=(const char* s);
    String& operator+=(const String& s);

    String operator-(const char* s) const;
    String operator-(const String& s) const;
    String& operator-=(const char* s);
    String& operator-=(const String& s);

    String& operator=(const char* s);
    String& operator=(const String& s);
    String& operator=(const char c);

    ~String();
};

}

#endif // HBCSTRING_H
