#pragma once

#include "JackCDevService.h"

namespace JackC
{
    //所有支持的配置文件中的配置项记录的类型
    /// 注意：若要添加新项，请同步添加对应字符串至s_configRecordTypeNames
    enum class ConfigRecordType : int32_t
    {
        InValid = 0,   ///< 无效类型
        String,        ///< 字符串类型
        Int,           ///< 整数类型
        Double,        ///< 双精度浮点数类型
        Num_Of_Types   ///< 支持的类型总数
    };

    //配置文件的一条配置项记录
    FINAL_INTERFACE class JACKC_COMMON_EXPORT IConfigRecord
    {
    public:
        //获取支持的配置项记录的类型名。
        static const std::vector<std::wstring>& GetConfigRecordTypeNames();

    public:
        //通过名字和字符串类型值创建配置项记录。
        static OwnerPtr<IConfigRecord> Create(const std::wstring& name, const std::wstring& stringValue);

        //通过名字和整数类型值创建配置项记录。
        static OwnerPtr<IConfigRecord> Create(const std::wstring& name, int intValue);

        //通过名字和字符串类型值创建配置项记录。
        static OwnerPtr<IConfigRecord> Create(const std::wstring& name, double doubleValue);

    public:  
        //克隆一份配置项记录对象。
        virtual OwnerPtr<IConfigRecord> Clone() const = 0;

    public:
        //获取配置项记录的类型。
        virtual ConfigRecordType GetType() const = 0;

        //获取配置项记录的名称。
        virtual std::wstring GetName() const = 0;

        //获取配置项记录的字符串类型值。
        virtual std::wstring GetValueAsString() const = 0;

        //获取配置项记录的整数类型值。
        virtual int GetValueAsInt() const = 0;

        //获取配置项记录的双精度浮点数类型值。
        virtual double GetValueAsDouble() const = 0;

        //获取配置项记录的任何类型值的字符串。
        virtual std::wstring GetStringOfValue() const = 0;

    public:
        //配置项记录是否为内部标识码。
        virtual bool IsInternalIdentifyCodeValue() const = 0;
    };
}
