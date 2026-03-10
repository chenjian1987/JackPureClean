#pragma once

#include "JackCDevService.h"
#include <string>

namespace JackC
{
    /// 用于保存全局、静态字符串。保证相同内容的全局、静态字符串内存唯一。避免相同内容的字符串多次构造、拷贝。
    /// 另外，字符串比较，比较内容性能较差，在保证相同内容的全局、静态字符串内存唯一的情况，可以直接通过比较地址来判断字符串是否相同。
    class UniqueString
    {
    public:
        JACKC_COMMON_EXPORT static const UniqueString* Find(const std::string& content, bool createIfNotFound = true);
        const std::string& GetContent() const { return m_content; }

    private:
        UniqueString(const std::string& content);

        std::string m_content;
    };

    /// 用于保存全局、静态字符串。保证相同内容的全局、静态字符串内存唯一。避免相同内容的字符串多次构造、拷贝。
    /// 另外，字符串比较，比较内容性能较差，在保证相同内容的全局、静态字符串内存唯一的情况，可以直接通过比较地址来判断字符串是否相同。
    class UniqueWString
    {
    public:
        JACKC_COMMON_EXPORT static const UniqueWString * Find(const std::wstring & content, bool createIfNotFound = true);
        const std::wstring & GetContent() const { return m_content; }

    private:
        UniqueWString(const std::wstring & content);

        std::wstring m_content;
    };
}
