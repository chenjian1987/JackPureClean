#include "ClassId.h"
#include <regex>
#include <ostream>
#include <string>

#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"

using namespace JackC;

namespace
{
    void RegulateWString(std::wstring& outputStr, const std::wstring& input)
    {
        outputStr.reserve(input.size() + 4);
        for (const auto& item : input)
        {
            if (!::iswspace(item))
                outputStr.push_back(toupper(item));
        }
    }

    // 是否符合{%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}格式
    bool IsValidRegulatedWString(const std::wstring & regulatedWString)
    {
        const wchar_t* pRegex = LR"(^\{[0-9A-F]{8}-([0-9A-F]{4}-){2}([0-9A-F]{2}){2}-([0-9A-F]{2}){6}\}$)";
        const std::wregex hexPattern(pRegex);
        return std::regex_match(regulatedWString, hexPattern);
    }
}

ClassId::ClassId(const std::wstring & wstr) : m_guid(JackCGuid::GetInvalid())
{
    if (!wstr.empty())
    {
        std::wstring regulatedWStr;
        RegulateWString(regulatedWStr, wstr);
        m_guid = JackCGuid(regulatedWStr);
    }
}

bool ClassId::operator==(const ClassId & another) const
{
    if (this == &another)
    {
        return true;
    }
    return JackCGuid::Equal(m_guid, another.m_guid);
}

bool ClassId::operator!=(const ClassId & another) const
{
    if (this == &another)
    {
        return false;
    }
    return !this->operator==(another);
}

bool ClassId::operator>(const ClassId & another) const
{
    return this->m_guid > another.m_guid;
}

bool ClassId::operator<(const ClassId & another) const
{
    return this->m_guid < another.m_guid; 
}

bool ClassId::IsValid() const
{
    return m_guid.IsValid();
}

std::wstring ClassId::AsWString() const
{
    return m_guid.ToWString();
}

void ClassId::SetByWString(const std::wstring & wstr)
{
    m_guid = JackCGuid(wstr);
}

std::size_t ClassIdHasher::operator()(const ClassId & key) const
{
    return key.GetJackCGuid().Hash();
}
