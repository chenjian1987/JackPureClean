#pragma once
#include "JackCAssert.h"
#include <cstdint>
#include <type_traits>

namespace JackC
{
    /// <summary>
    /// 萃取类型真实名称
    /// 使用该宏可以让通过typedef定义的类型共享一个固定的名字，
    /// 通过JackC::TypeDefTrait<T>::RealName(L"T")获取。
    /// 若类型T及其typedef类型未为定义过该宏，则获取的名字和类型名拼写一样。 
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template <typename T>
    struct TypeDefTrait
    {
        static const wchar_t* RealName(const wchar_t* alias)
        {
            return alias;
        }
    };

    /// <summary>
    /// 判断给定类型是否是整型
    /// 和std::is_integral的区别是这里只甄别int，不包括bool和char
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template <typename T>
    struct IsIntegral : std::integral_constant<bool,
        std::is_same<T, short>::value ||
        std::is_same<T, unsigned short>::value ||
        std::is_same<T, int>::value ||
        std::is_same<T, unsigned int>::value ||
        std::is_same<T, long>::value ||
        std::is_same<T, unsigned long>::value ||
        std::is_same<T, long long>::value ||
        std::is_same<T, unsigned long long>::value>
    {};

    //整型、浮点类型
    enum class EnumIntOrFloatType : uint8_t
    {
        UnKnown = 0,    ///< 未知
        Float,          ///< 单精度浮点数
        Double,         ///< 双精度浮点数
        UInt8,          ///< 8位无符号整数
        UInt16,         ///< 16位无符号整数
        UInt32,         ///< 32位无符号整数
        UInt64,         ///< 64位无符号整数
        Int8,           ///< 8位有符号整数
        Int16,          ///< 16位有符号整数
        Int32,          ///< 32位有符号整数
        Int64,          ///< 64位有符号整数
    };

    //整型的进制类型
    enum class EnumIntBaseType : uint8_t
    {
        UnKnown = 0,    ///< 未知
        Dec,            ///< 十进制
        Bin,            ///< 二进制
        Oct,            ///< 八进制
        Hex,            ///< 十六进制
    };
}

//////////////////////////////////////////////////////////////////////////
//值类型的类型(详细类型参见JackC::property_is_vaue_type)，需要定义可视化类型（EnumIntOrFloatType）和进制（BaseOfInt）。
// 缺少该定义编译时会报错提示。

// 可视化类型是整型
#define DEFINE_VISUAL_TYPE_INVALID(BASE)  static JackC::EnumIntOrFloatType GetVisualType(JackC::EnumIntBaseType& base) { base = BASE; return JackC::EnumIntOrFloatType::UnKnown; }
#define DEFINE_VISUAL_TYPE_UINT8(BASE)  static JackC::EnumIntOrFloatType GetVisualType(JackC::EnumIntBaseType& base) { base = BASE; return JackC::EnumIntOrFloatType::UInt8; }
#define DEFINE_VISUAL_TYPE_UINT16(BASE) static JackC::EnumIntOrFloatType GetVisualType(JackC::EnumIntBaseType& base) { base = BASE; return JackC::EnumIntOrFloatType::UInt16; }
#define DEFINE_VISUAL_TYPE_UINT32(BASE)  static JackC::EnumIntOrFloatType GetVisualType(JackC::EnumIntBaseType& base) { base = BASE; return JackC::EnumIntOrFloatType::UInt32; }
#define DEFINE_VISUAL_TYPE_UINT64(BASE)  static JackC::EnumIntOrFloatType GetVisualType(JackC::EnumIntBaseType& base) { base = BASE; return JackC::EnumIntOrFloatType::UInt64; }
#define DEFINE_VISUAL_TYPE_INT8(BASE)  static JackC::EnumIntOrFloatType GetVisualType(JackC::EnumIntBaseType& base) { base = BASE; return JackC::EnumIntOrFloatType::Int8; }
#define DEFINE_VISUAL_TYPE_INT16(BASE)  static JackC::EnumIntOrFloatType GetVisualType(JackC::EnumIntBaseType& base) { base = BASE; return JackC::EnumIntOrFloatType::Int16; }
#define DEFINE_VISUAL_TYPE_INT32(BASE)  static JackC::EnumIntOrFloatType GetVisualType(JackC::EnumIntBaseType& base) { base = BASE; return JackC::EnumIntOrFloatType::Int32; }
#define DEFINE_VISUAL_TYPE_INT64(BASE)  static JackC::EnumIntOrFloatType GetVisualType(JackC::EnumIntBaseType& base) { base = BASE; return JackC::EnumIntOrFloatType::Int64; }

// 规范enum class的写法
#define ENUM_CLASS(ENUM_TYPE) enum class ENUM_TYPE :int32_t

// 可视化类型是浮点型
#define DEFINE_VISUAL_TYPE_FLOAT()  static JackC::EnumIntOrFloatType GetVisualType(JackC::EnumIntBaseType&) { return JackC::EnumIntOrFloatType::Float; }
#define DEFINE_VISUAL_TYPE_DOUBLE()  static JackC::EnumIntOrFloatType GetVisualType(JackC::EnumIntBaseType&) { return JackC::EnumIntOrFloatType::Double; }

//////////////////////////////////////////////////////////////////////////
//TypeDefTrait的通用实例化模板
#ifdef __linux__
#define IMMORTAL_TYPE_NAME(T) \
    template <> \
    struct TypeDefTrait<T> \
    { \
        static const wchar_t* RealName(const wchar_t*) \
        { \
            return WSTRING_OF(T); \
        } \
    };
#else
#define IMMORTAL_TYPE_NAME(T) \
    template <> \
    struct JackC::TypeDefTrait<T> \
    { \
        static const wchar_t* RealName(const wchar_t*) \
        { \
            return WSTRING_OF(T); \
        } \
    };
#endif

    //转换旧值类型并赋给新值，只支持整型和浮点型间的转换
template <typename OLD_TYPE, typename NEW_TYPE>
struct TypeCastTrait
{
    static_assert(std::is_integral<OLD_TYPE>::value || std::is_floating_point<OLD_TYPE>::value,
        "OLD_TYPE should be only integer or floating");
    static_assert(std::is_integral<NEW_TYPE>::value || std::is_floating_point<NEW_TYPE>::value,
        "NEW_TYPE should be only integer or floating");

    static void StaticCast(const char* pOldValue, char*& pNewValue)
    {
        OLD_TYPE oldValue = *(OLD_TYPE*)pOldValue;
        *(NEW_TYPE*)pNewValue = static_cast<NEW_TYPE>(oldValue);
    }
};

//将S类型的数值转换为T类型的。如果发生数据丢失，报错并返回T()。
#ifdef _DEBUG
template <typename T, typename S>
T SafeTypeCast(S sourceValue)
{
    T targetValue = static_cast<T>(sourceValue);
    JACKC_ASSERT(targetValue == sourceValue && L"类型转换存在数据丢失");
    return targetValue;
}
#else
#define SafeTypeCast static_cast
#endif

    // 整型类型的数据定义宏
#define DEFINE_FUNDAMENTAL_INT_TYPE(ALIAS, TYPE) \
    IMMORTAL_TYPE_NAME(TYPE); typedef TYPE ALIAS;

    // 基础整型的类型定义
#if defined(__linux__)
template <>
struct JackC::TypeDefTrait<uint8_t>
{
    static const wchar_t* RealName(const wchar_t*)
    {
        return L"uint8_t";
    }
};
typedef uint8_t UInt8;

template <>
struct JackC::TypeDefTrait<uint16_t>
{
    static const wchar_t* RealName(const wchar_t*)
    {
        return L"uint16_t";
    }
};
typedef uint16_t UInt16;

template <>
struct JackC::TypeDefTrait<uint32_t>
{
    static const wchar_t* RealName(const wchar_t*)
    {
        return L"uint32_t";
    }
};
typedef uint32_t UInt32;

template <>
struct JackC::TypeDefTrait<uint64_t>
{
    static const wchar_t* RealName(const wchar_t*)
    {
        return L"uint64_t";
    }
};
typedef uint64_t UInt64;

template <>
struct JackC::TypeDefTrait<int8_t>
{
    static const wchar_t* RealName(const wchar_t*)
    {
        return L"int8_t";
    }
};
typedef int8_t Int8;

template <>
struct JackC::TypeDefTrait<int16_t>
{
    static const wchar_t* RealName(const wchar_t*)
    {
        return L"int16_t";
    }
};
typedef int16_t Int16;

template <>
struct JackC::TypeDefTrait<int32_t>
{
    static const wchar_t* RealName(const wchar_t*)
    {
        return L"int32_t";
    }
};
typedef int32_t Int32;

template <>
struct JackC::TypeDefTrait<int64_t>
{
    static const wchar_t* RealName(const wchar_t*)
    {
        return L"int64_t";
    }
};
typedef int64_t Int64;

template <>
struct JackC::TypeDefTrait<float>
{
    static const wchar_t* RealName(const wchar_t*)
    {
        return L"float";
    }
};

template <>
struct JackC::TypeDefTrait<double>
{
    static const wchar_t* RealName(const wchar_t*)
    {
        return L"double";
    }
};
#else
DEFINE_FUNDAMENTAL_INT_TYPE(UInt8, uint8_t);
DEFINE_FUNDAMENTAL_INT_TYPE(UInt16, uint16_t);
DEFINE_FUNDAMENTAL_INT_TYPE(UInt32, uint32_t);
DEFINE_FUNDAMENTAL_INT_TYPE(UInt64, uint64_t);
DEFINE_FUNDAMENTAL_INT_TYPE(Int8, int8_t);
DEFINE_FUNDAMENTAL_INT_TYPE(Int16, int16_t);
DEFINE_FUNDAMENTAL_INT_TYPE(Int32, int32_t);
DEFINE_FUNDAMENTAL_INT_TYPE(Int64, int64_t);

// float和double长度大小是固定的
IMMORTAL_TYPE_NAME(float);
IMMORTAL_TYPE_NAME(double);
#endif

/// <summary>
/// 字节类型，不推荐使用char。自定义成Byte后固定大小为8bit。
/// c++未对char的长度有实现上的强制要求，只要求： 1 <= sizeof(char) <= 
/// sizeof(short) <= sizeof(int) <= sizeof(long) <= sizeof(long long)
/// http://en.cppreference.com/w/cpp/language/types
/// </summary>
struct JACKC_COMMON_EXPORT Byte
{
    DEFINE_VISUAL_TYPE_UINT8(JackC::EnumIntBaseType::Bin)
    typedef Int8 value_type;
    Int8 Value;
    Byte() : Value(0) { }
    Byte(char c) { Value = (Int8)c; }
    operator char() const { return (char)Value; }
};

/// <summary>
/// bool类型实现未规定，大小可变。自定义成Bool后固定大小为8bit。
/// </summary>
struct JACKC_COMMON_EXPORT Bool
{
    DEFINE_VISUAL_TYPE_UINT8(JackC::EnumIntBaseType::Dec)
    typedef uint8_t value_type;
    uint8_t Value;
    Bool() : Value(0) { }
    Bool(bool v) { Value = (v ? 1 : 0); }
    operator bool() const { return (Value == 1) ? true : false; }
};


/// <summary>
/// wchar_t类型实现未规定，大小可变。自定义成WChar后固定大小为32bit。
/// </summary>
struct JACKC_COMMON_EXPORT WChar
{
    DEFINE_VISUAL_TYPE_UINT32(JackC::EnumIntBaseType::Hex)
    typedef uint32_t value_type;
    uint32_t Value;
    WChar() : Value(0) { }
    WChar(wchar_t v) { Value = v; }
    operator wchar_t() const { return (wchar_t)Value; }
};

