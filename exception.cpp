#include "exception.h"
#include <cstring>
#include <cstdlib>

using namespace std;

namespace HBCLib
{

void Exception::init(const char* message, const char* File, int Line)
{
    m_message = strdup(message);

    if (File != NULL)
    {
        char sl[16] = {0};

        itoa(Line,sl,10);

        m_location = static_cast<char*> (malloc(strlen(sl) + strlen(File) + 2));

        if (m_location != NULL)
        {
            m_location = strcpy(m_location,File);
            m_location = strcat(m_location,":");
            m_location = strcat(m_location,sl);
        }
    }
    else
    {
        m_location = NULL;
    }
}

Exception::Exception(const char* message)
{
    init(message, NULL, 0);
}

Exception::Exception(const char* File, int Line)
{
    init(NULL, File, Line);
}

Exception::Exception(const char* message, const char* File, int Line)
{
    init(message, File, Line);
}

Exception::Exception(const Exception& ex)
{
    m_location = strdup(ex.m_location);
    m_message = strdup(ex.m_message);
}

Exception& Exception::operator=(const Exception& ex)
{
    if (this != &ex)
    {
        free(m_location);
        free(m_message);

        m_location = strdup(ex.m_location);
        m_message = strdup(ex.m_message);
    }
    return * this;
}

const char* Exception::message()const
{
    return m_message;
}

const char* Exception::location()const
{
    return m_location;
}

Exception::~Exception()
{
    free(m_location);
    free(m_message);
}

}
