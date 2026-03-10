#pragma once

#include "JackCDevService.h"

// 禁止有关STL无法导出的警告误报
#pragma warning( push )
#pragma warning( disable : 4251 )

namespace JackC
{
    // JackCString相当于StringBuilder和ostringstream
    class JACKC_COMMON_EXPORT JackCString 
    {
    public:
        JackCString();
        JackCString(const wchar_t* pStrW);
        JackCString(const std::wstring& strW);

        // ！！注意！！ isNative只有和系统编码相关的转码操作才建议使用
        JackCString(const char* pStr, bool isNative = true);
        JackCString(const std::string& str, bool isNative = true);

    // 各种操作符重载
    public:
        JackCString operator + (const JackCString& _other) const;
        JackCString& operator += (const JackCString& _other);
        bool operator < (const JackCString& _other) const { return getString() < _other.getString(); }
        bool operator == (const JackCString& _other) const { return getString() == _other.getString(); }
        bool operator != (const JackCString& _other) const { return getString() != _other.getString(); }
        JackCString& operator << (const JackCString& valString);
        JackCString& operator << (int valInt);
        JackCString& operator << (long long valInt64);
        JackCString& operator << (double valDouble);

    // 与std::wstring对应的各种字符串操作
    public:
        static const int npos;
        static const JackCString endl;
        bool empty() const { return getString().empty(); }
        int size() const { return (int)getString().size(); }
        int find(const JackCString& _other, int pos) const { return (int)getString().find(_other.getString(), pos); }
        JackCString substr(int pos, int n) const;

        JackCString& assign(const JackCString& _other);
        JackCString& replace(int pos, int size, const JackCString& newStr);
        void clear() { getStringFW().clear(); }

    // 自有操作
    public:
        void ReplaceSubString(const JackCString& oldSubString, const JackCString& newSubString);
        void RemoveQuotationMark();

    // 不同字符串的输出接口
    public:
        const std::wstring& getString() const { return m_theString; }
        std::wstring& getStringFW() { return m_theString; }
        const wchar_t* c_str() const { return getString().c_str(); }

    private:
        std::wstring m_theString;
    };
}

#pragma warning( pop )
