#pragma once

#include "JackCDevService.h"
#include <string>
#include <sstream>
#include <iomanip>

namespace JackC
{
    class JACKC_COMMON_EXPORT StringUtils
    {
    public:
        enum EnCodeType
        {
            CT_ANSI = 0,
            CT_UTF8 = 1
            //// Add more supported types here
        };

        template <typename T>
        inline static std::wstring ToWString(T t)
        {
            static_assert(
                std::is_same<T, short>::value ||
                std::is_same<T, unsigned short>::value ||
                std::is_same<T, int>::value ||
                std::is_same<T, unsigned int>::value ||
                std::is_same<T, long>::value ||
                std::is_same<T, unsigned long>::value ||
                std::is_same<T, long long>::value ||
                std::is_same<T, unsigned long long>::value,
                "Only accept integers");

            std::wostringstream woss;
            woss << t;
            return woss.str();
        }

        template <typename T>
        inline static std::string ToString(T t)
        {
            static_assert(
                std::is_same<T, short>::value ||
                std::is_same<T, unsigned short>::value ||
                std::is_same<T, int>::value ||
                std::is_same<T, unsigned int>::value ||
                std::is_same<T, long>::value ||
                std::is_same<T, unsigned long>::value ||
                std::is_same<T, long long>::value ||
                std::is_same<T, unsigned long long>::value,
                "Only accept integers");

            std::ostringstream oss;
            oss << t;
            return oss.str();
        }

        // 精确到小数点后precision位
        inline static std::wstring ToWString(double t, int precision = 20)
        {
            std::wostringstream woss;
            woss << std::fixed << std::setprecision(precision) << t;
            return woss.str();
        }

        // 精确到小数点后precision位
        inline static std::wstring ToWString(float t, int precision = 6)
        {
            std::wostringstream woss;
            woss << std::fixed << std::setprecision(precision) << t;
            return woss.str();
        }

        static std::wstring ToWString(bool value);

        template <typename T>
        inline static T ToNumber(const std::wstring& str, bool* pOk = nullptr)
        {
            static_assert(
                std::is_same<T, short>::value ||
                std::is_same<T, unsigned short>::value ||
                std::is_same<T, int>::value ||
                std::is_same<T, unsigned int>::value ||
                std::is_same<T, long>::value ||
                std::is_same<T, unsigned long>::value ||
                std::is_same<T, long long>::value ||
                std::is_same<T, unsigned long long>::value ||
                std::is_same<T, float>::value ||
                std::is_same<T, double>::value,
                "Only accept Numbers");

            std::wistringstream wiss(TrimSpace(str));
            T result;
            wiss >> result;
            if (pOk)
            {
                *pOk = wiss.eof() && !wiss.bad() && !wiss.fail();   // eof正常结束， bad/fail对付空字符串
            }
            return result;
        }

        template <typename T>
        inline static T ToNumber(const std::string& str, bool* pOk = nullptr)
        {
            static_assert(
                std::is_same<T, short>::value ||
                std::is_same<T, unsigned short>::value ||
                std::is_same<T, int>::value ||
                std::is_same<T, unsigned int>::value ||
                std::is_same<T, long>::value ||
                std::is_same<T, unsigned long>::value ||
                std::is_same<T, long long>::value ||
                std::is_same<T, unsigned long long>::value ||
                std::is_same<T, float>::value ||
                std::is_same<T, double>::value,
                "Only accept Numbers");

            std::istringstream iss(TrimSpace(str));
            T result;
            iss >> result;
            if (pOk)
            {
                *pOk = iss.eof() && !iss.bad() && !iss.fail();   // eof正常结束， bad/fail对付空字符串
            }
            return result;
        }

        static bool ToBool(const std::string& value);
        static bool ToBool(const std::wstring& value);

        // Use UTF-8 by default
        static std::wstring ToWString(const std::string& src);

        // Use UTF-8 by default
        static std::wstring ToWString(const char* pSrc);

        // TODO 下面的重载不加的话，clang会特化上面的模板
        static std::wstring ToWString(char* pSrc);

        // Use UTF-8 by default
        static std::string ToString(const std::wstring& src);

        // Use UTF-8 by default
        static std::string ToString(const wchar_t* pSrc);
        static std::string ToString(wchar_t* pSrc)
        {
            return ToString((const wchar_t*)pSrc);
        }

        // 转大小写
        static std::wstring WStringToLower(const std::wstring& sVal);
        static std::wstring WStringToUpper(const std::wstring& sVal);

        // 如文件路径、命令行参数等和本地系统编码相关，才建议使用
        static std::string ToStringByCodeType(const std::wstring& src, EnCodeType codeType);
        static std::wstring ToWStringByCodeType(const std::string& src, EnCodeType codeType);

        static void ReplaceAll(std::wstring& inputStr, const std::wstring& oldValue, const std::wstring& newValue);

        // trim：是否去除子串的首尾空白字符，返回字符串个数=分隔符数+1
        static std::vector<std::wstring> SplitString(const std::wstring& strData, const std::wstring& seperator, bool trimSpace = false);
        static std::vector<std::string> SplitString(const std::string& strData, const std::string& seperator, bool trimSpace = false);

        // 返回移除首尾空白字符的字符串
        static std::wstring TrimSpace(const std::wstring& str);
        static std::string TrimSpace(const std::string& str);

        // 返回移除首尾指定字符的字符串
        static std::wstring Trim(const std::wstring& str, std::function<bool(wchar_t)> func);
        static bool EndWith(const std::wstring& str, const std::wstring& endStr);
        static bool StartWith(const std::wstring& str, const std::wstring& startStr);

        static std::wstring CollapseToOneLine(const std::wstring& inputStr, const std::wstring& lineSpliter = L" | ");
        static std::wstring FormatLogParams(const std::wstring& inputStr);

        static std::wstring FormatWString(const std::wstring fmt_str, ...);

        //from llvm
        template <class... Args>
        static std::wstring StrCat(Args&&... args)
        {
            std::wostringstream ss;
            StrCatImp(ss, std::forward<Args>(args)...);
            return ss.str();
        }

    private:
        static std::wostream& StrCatImp(std::wostream& out)
        {
            return out;
        }

        template <class First, class... Rest>
        static std::wostream& StrCatImp(std::wostream& out, First&& f,
            Rest&&... rest) 
        {
            out << std::forward<First>(f);
            return StrCatImp(out, std::forward<Rest>(rest)...);
        }
    };

    // 忽略大小写的字符串比较函数。
    // 是StringCompareUtil::CompareInogreCase的语法糖
    int JACKC_COMMON_EXPORT StrICmp(const char* pLHS, const char* pRHS);
    int JACKC_COMMON_EXPORT StrICmp(const wchar_t* pLHS, const wchar_t* pRHS);
} // namespace