#include "JackCString.h"
#include <sstream>
#include <iomanip>
#include "StringUtils.h"
#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"
using namespace JackC;

const int JackCString::npos = (int)std::wstring::npos;
const JackCString JackCString::endl = L"\n\r";

JackCString::JackCString()
{

}

JackCString::JackCString(const std::wstring& strW)
    : m_theString(strW)
{
}

JackCString::JackCString(const wchar_t* pStrW)
    : m_theString(pStrW)
{
}

JackCString::JackCString(const std::string& str, bool isNative)
{
    if (isNative)
    {
        m_theString = StringUtils::ToWStringByCodeType(str, StringUtils::CT_ANSI);
    }
    else
    {
        m_theString = StringUtils::ToWString(str);
    }
}

JackCString::JackCString(const char* pStr, bool isNative)
{
    if (isNative)
    {
        m_theString = StringUtils::ToWStringByCodeType(pStr, StringUtils::CT_ANSI);
    }
    else
    {
        m_theString = StringUtils::ToWString(pStr);
    }
}

JackCString JackCString::operator + (const JackCString& _other) const
{
    JackCString rtStr;
    rtStr = *this;
    rtStr += _other;
    return rtStr;
}

JackCString& JackCString::operator += (const JackCString& _other)
{
    getStringFW() += _other.getString();
    return *this;
}

JackCString& JackCString::assign(const JackCString& _other)
{
    getStringFW().assign(_other.getString());
    return *this;
}

JackCString& JackCString::replace(int pos, int size, const JackCString& newStr)
{
    getStringFW().replace(pos, size, newStr.getString());
    return *this;
}

void JackCString::ReplaceSubString(const JackCString& oldSubString, const JackCString& newSubString)
{
    int pos = 0;
    int a = oldSubString.size();
    int b = newSubString.size();
    while((pos = find(oldSubString, pos)) != JackCString::npos)
    {
        replace(pos, a, newSubString);
        pos += b;
    }
}

void JackCString::RemoveQuotationMark()
{
    while(!m_theString.empty() && m_theString.front() == L'\"')
    {
        m_theString = m_theString.substr(1, m_theString.length() - 1);
    }
    while(!m_theString.empty() && m_theString.back() == L'\"')
    {
        m_theString = m_theString.substr(0, m_theString.length() - 1);
    }
}

JackCString& JackCString::operator << (const JackCString& _other)
{
    m_theString += _other.m_theString;
    return *this;
}

JackCString& JackCString::operator << (int valInt)
{
    m_theString += StringUtils::ToWString(valInt);
    return *this;
}

JackCString& JackCString::operator << (long long valInt64)
{
    m_theString += StringUtils::ToWString(valInt64);
    return *this;
}

JackCString& JackCString::operator << (double valDouble)
{
    m_theString += StringUtils::ToWString(valDouble, 20);
    return *this;
}

JackCString JackCString::substr(int pos, int n) const
{
    return m_theString.substr(pos, n);
}

