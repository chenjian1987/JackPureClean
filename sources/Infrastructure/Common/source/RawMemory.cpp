#include "RawMemory.h"
#include "DebugMessage.h"
#include <cstring>

#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"

using namespace JackC;

RawMemory::RawMemory(size_t size, bool zeroMemory)
    :m_ptr(nullptr)
    ,m_size(size)
    ,m_constructed(false)
{
    if (m_size > 0)
    {
        m_ptr = new char[size];
        if (zeroMemory)
        {
            std::memset(m_ptr, '\0', (size_t)size);
        }    
    }
}

RawMemory::RawMemory() 
    :m_ptr(nullptr)
    , m_size(0)
    , m_constructed(false)
{

}

RawMemory::~RawMemory()
{
    if (m_ptr)
    {
        delete[] m_ptr;
        m_ptr = nullptr;
    }
}

void RawMemory::Resize(size_t size)
{
    if (m_ptr)
    {
        delete[] m_ptr;
        m_ptr = nullptr;
    }
    if (size != 0)
    {
        m_ptr = new char[size];
        std::memset(m_ptr, '\0', (size_t)size);
        m_size = size;
    }
}

bool RawMemory::IsZeroMemory() const
{
    for (int ii = 0; ii < m_size; ++ii)
    {
        if (m_ptr[ii] != 0)
        {
            return false;
        }    
    }
    return true;
}
