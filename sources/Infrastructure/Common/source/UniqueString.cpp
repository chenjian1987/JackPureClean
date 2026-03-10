#include "UniqueString.h"
#include <mutex>
#include "JackCNew.h"
#include <unordered_map>
#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"
using namespace JackC;


UniqueString::UniqueString(const std::string& content) : m_content(content)
{
}

namespace
{
    std::mutex s_uniqueWStringsMutex;
    std::mutex s_uniqueStringsMutex;
}

// 支持多线程时需要考虑线程安全。
const UniqueString* UniqueString::Find(const std::string& content, bool createIfNotFound /*= true*/)
{
    std::lock_guard<std::mutex> lockGuard(s_uniqueStringsMutex);
    static std::unordered_map<std::string, OwnerPtr<UniqueString>> s_uniqueStrings;
    std::unordered_map<std::string, OwnerPtr<UniqueString>>::const_iterator it = s_uniqueStrings.find(content);
    if (it != s_uniqueStrings.end())
    {
        return it->second.get();
    }

    if (createIfNotFound)
    {
        s_uniqueStrings.emplace(std::make_pair(content, NEW_AS_OWNER_PTR(UniqueString, content)));
        std::unordered_map<std::string, OwnerPtr<UniqueString>>::const_iterator iter = s_uniqueStrings.find(content);
        return iter->second.get();
    }
    return nullptr;
}


UniqueWString::UniqueWString(const std::wstring& content) : m_content(content)
{
}


const UniqueWString* UniqueWString::Find(const std::wstring& content, bool createIfNotFound /*= true*/)
{
    std::lock_guard<std::mutex> lockGuard(s_uniqueWStringsMutex);
    static std::unordered_map<std::wstring, OwnerPtr<UniqueWString>> s_uniqueWStrings;
    auto it = s_uniqueWStrings.find(content);
    if (it != s_uniqueWStrings.end())
    {
        return it->second.get();
    }

    if (createIfNotFound)
    {
        s_uniqueWStrings.emplace(std::make_pair(content, NEW_AS_OWNER_PTR(UniqueWString, content)));
        std::unordered_map<std::wstring, OwnerPtr<UniqueWString>>::const_iterator iter = s_uniqueWStrings.find(content);
        return iter->second.get();
    }

    return nullptr;
}
