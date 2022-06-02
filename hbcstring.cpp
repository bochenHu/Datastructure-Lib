#include "hbcstring.h"
#include "exception.h"
#include <cstring>
#include <cstdlib>

namespace HBCLib
{

void String::init(const char* s)
{
    m_str = strdup(s);

    if (m_str)
    {
        m_length = strlen(m_str);
    }
    else
    {
        EXCEPTION_THROW(NoEnoughMemoryException,"no enough memory to initiate String Object...");
    }
}

bool String::compare(const char* a,const char* b, int len) const
{
    bool ret = true;
    for (int i = 0; i < len && ret; ++i)
    {
        ret = (ret) && (a[i] == b[i]);
    }

    return ret;
}

int* String::make_pmt(const char* p) const  //O(m)
{
    int len = strlen(p);
    int* pmt = static_cast<int*>(malloc(sizeof(int) * len));

    if (pmt)
    {
        int ll = 0;
        pmt[0] = 0;


        for (int i = 1; i < len; ++i)
        {
            while ((ll > 0) && (p[ll] != p[i]))
            {
                ll = pmt[ll-1];
            }

            if (p[ll] == p[i])
            {
                ll++;
            }

            pmt[i] = ll;
        }
    }

    return pmt;
}

int String::kmp(const char* s, const char* p) const  //O(m+n)
{
    int ret = -1;
    int ls = strlen(s);
    int lp = strlen(p);
    int* kmp = make_pmt(p);

    if ((kmp) && (lp <= ls) && (0 < lp))
    {
        for (int i = 0, j = 0; i < ls; ++i)
        {
            while ((j > 0) && (s[i] != p[j]))
            {
                j = kmp[j-1];
            }

            if (s[i] == p[j])
            {
                j++;
            }

            if (j == lp)
            {
                ret = i + 1 - lp;
                break;
            }
        }
    }

    free(kmp);

    return ret;
}

String::String()
{
    init("");
}

String::String(const char* s)
{
    init(s ? s : "");
}

String::String(const String& t)
{
    init(t.m_str);
}

String::String(const char c)
{
    char s[] = {c,'\0'};
    init(s);
}

int String::length() const
{
    return m_length;
}

const char* String::str() const
{
    return m_str;
}

char String::operator[](int i) const
{
    return const_cast<String&>(*this)[i];
}

char& String::operator[](int i)
{
    if (0 <= i && i < m_length)
    {
        return m_str[i];
    }
    else
    {
        EXCEPTION_THROW(IndexOutOfBoundsException,"invalid parameter i...");
    }
}

bool String::startWith(const char* s) const
{
    bool ret = (s != NULL);
    if (ret)
    {
        int len = strlen(s);

        ret = (len < m_length) && (compare(s,m_str,len));
    }
    return ret;
}

bool String::startWith(const String& s) const
{
    return startWith(s.m_str);
}

bool String::endOf(const char* s) const
{
    bool ret = (s != NULL);
    if (ret)
    {
        int len = strlen(s);
        char* str = m_str + m_length - len;

        ret = (len < m_length) && (compare(s,str,len));
    }
    return ret;
}

bool String::endOf(const String& s) const
{
    return endOf(s.m_str);
}

String& String::insert(int i, const char* s)
{
    if (0 <= i && i < m_length)
    {
        if ((s != NULL) && (s[0] != '\0'))
        {
            int len = strlen(s);

            char*  str = reinterpret_cast<char*>(malloc(len + m_length + 1));

            if (str)
            {
                strncpy(str,m_str,i);
                strncpy(str+i,s,len);
                strncpy(str+i+len,m_str + i,m_length - i);
                str[len + m_length] = '\0';

                free(m_str);
                m_length = m_length + len;
                m_str = str;
            }
            else
            {
                EXCEPTION_THROW(NoEnoughMemoryException,"no enough memory to insert string values...");
            }
        }
    }
    else
    {
        EXCEPTION_THROW(IndexOutOfBoundsException,"invalid parameter i...");
    }

    return *this;
}

String& String::insert(int i, const String& s)
{
    return insert(i,s.m_str);
}

String& String::trim()
{
    int a = 0;
    int b = m_length - 1;

    while ((*this)[a] == ' ') a++;
    while ((*this)[b] == ' ') b--;

    if (a == 0)
    {
        m_str[b+1] = '\0';
    }
    else
    {
        for (int i = 0, j = a; i < b-a+1; ++i, ++j)
        {
            m_str[i] = m_str[j];
        }

        m_str[b-a+1] = '\0';
    }

    m_length = b-a+1;

    return *this;
}

int String::indexOf(const char* s) const
{
    return kmp(m_str,s ? s : "");
}

int String::indexOf(const String& s) const
{
    return kmp(m_str,s.m_str);
}

String& String::remove(int i, int len)
{
    if ((0 <= i) && (i < m_length))
    {
        int a = i;
        int b = i + len;

        while ((b > a) && (b < m_length))
        {
            m_str[a++] = m_str[b++];
        }

        m_str[a] = '\0';
        m_length = a;
    }

    return *this;
}

String& String::remove(const char* s)
{
    return remove(indexOf(s),s ? strlen(s): 0);
}

String& String::remove(const String& s)
{
    return remove(indexOf(s.m_str),s.m_length);
}

String& String::replace(const char* t, const char* s)
{
    int index = indexOf(t);

    if (index >= 0)
    {
        remove(t);
        insert(index,s);
    }

    return *this;
}

String& String::replace(const char* t, const String s)
{
    return replace(t,s.m_str);
}

String& String::replace(const String t, const char* s)
{
    return replace(t.m_str,s);
}

String& String::replace(const String t, const String s)
{
    return replace(t.m_str,s.m_str);
}

String String::sub(int i, int len) const
{
    String ret;
    if ((0 <= i) && (i < m_length))
    {
        if (len < 0) len = 0;
        if (len > m_length) len = m_length - i;

        char* str = reinterpret_cast<char*>(malloc(len + 1));

        strncpy(str, m_str+i, len);
        str[len] = '\0';

        free(ret.m_str);
        ret.m_str = str;
        ret.m_length = len;
    }

    return ret;
}

bool String::operator==(const char* s) const
{
    return (strcmp(m_str,s ? s : "") == 0);
}

bool String::operator==(const String& s) const
{
    return (strcmp(m_str,s.m_str) == 0);
}

bool String::operator!=(const char* s) const
{
    return !(*this == s);
}

bool String::operator!=(const String& s) const
{
    return !(*this == s);
}

bool String::operator>(const char* s) const
{
    return (strcmp(m_str,s ? s : "") > 0);
}

bool String::operator>(const String& s) const
{
    return (strcmp(m_str,s.m_str));
}

bool String::operator<(const char* s) const
{
    return (strcmp(m_str,s ? s : "") < 0);
}

bool String::operator<(const String& s) const
{
    return (strcmp(m_str,s.m_str) < 0);
}

bool String::operator>=(const char* s) const
{
    return (strcmp(m_str,s ? s : "") >= 0);
}

bool String::operator>=(const String& s) const
{
    return (strcmp(m_str,s.m_str) >= 0);
}

bool String::operator<=(const char* s) const
{
    return (strcmp(m_str,s ? s : "") <= 0);
}

bool String::operator<=(const String& s) const
{
    return (strcmp(m_str,s.m_str) <= 0);
}

String String::operator+(const char* s) const
{
    String ret;
    int len = m_length + strlen(s ? s : "");
    char* str = reinterpret_cast<char*>(malloc(len+1));

    if (str)
    {
        strcpy(str,m_str);
        strcat(str,s);

        free(ret.m_str);

        ret.m_str = str;
        ret.m_length = len;
    }
    else
    {
        EXCEPTION_THROW(NoEnoughMemoryException,"no enough memory to add string values...");
    }

    return ret;
}

String String::operator+(const String& s) const
{
    return (*this + s.m_str);
}

String& String::operator+=(const char* s)
{
    return (*this = *this + s);
}

String& String::operator+=(const String& s)
{
    return (*this = *this + s);
}

String String::operator-(const char* s) const
{
    return String(*this).remove(s);
}

String String::operator-(const String& s) const
{
    return String(*this).remove(s);
}

String& String::operator-=(const char* s)
{
    return remove(s);
}

String& String::operator-=(const String& s)
{
    return remove(s);
}

String& String::operator=(const char* s)
{
    if (this->m_str != s)
    {
        char* temp = strdup(s ? s : "");

        if (temp)
        {

            free(m_str);

            m_str = temp;
            m_length = strlen(s);
        }
        else
        {
            EXCEPTION_THROW(NoEnoughMemoryException,"no enough memory to copy a string object...");
        }
    }
    return *this;
}

String& String::operator=(const String& s)
{
    return (*this = s.m_str);
}

String& String::operator=(const char c)
{
    char s[] = {c,'\0'};

    return (*this = s);
}


String::~String()
{
    free(m_str);
}

}

