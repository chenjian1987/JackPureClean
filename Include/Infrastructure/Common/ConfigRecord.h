#pragma once

#include "JackCDevService.h"
#include "IConfigRecord.h"

#pragma warning( push )
#pragma warning( disable : 4251 )

namespace JackC
{
    ////////////////////////////////////////////////////////////////////////
    // 配置文件的一条记录，由类型、名称和值三项组成
    // 方便起见，每条记录内部都有valueString, valueInt和valueDouble三个字段
    class JACKC_COMMON_EXPORT ConfigRecord : public IConfigRecord
    {
    public:
        virtual ~ConfigRecord();

    public:
        ConfigRecord(ConfigRecordType type, const std::wstring& name, const std::wstring& stringValue);
        ConfigRecord(ConfigRecordType type, const std::wstring& name, int intValue);
        ConfigRecord(ConfigRecordType type, const std::wstring& name, double doubleValue);
        
    public:
        virtual OwnerPtr<IConfigRecord> Clone() const override;

    public:
        ConfigRecord(const ConfigRecord& other);
        ConfigRecord& operator= (const ConfigRecord& other);

    public:
        virtual ConfigRecordType GetType() const override { return m_type; }

        virtual std::wstring GetName() const override { return m_name; }

    public:
        virtual std::wstring GetValueAsString() const override;

        virtual int GetValueAsInt() const override;

        virtual double GetValueAsDouble() const override;

        // 仅用于配置XML文件保存时
        virtual std::wstring GetStringOfValue() const override;

    public:
        // 配置项记录是否为内部标识码
        virtual bool IsInternalIdentifyCodeValue() const override;

    private:
        ConfigRecordType m_type;
        std::wstring m_name;
        
        std::wstring m_valueString;
        int m_valueInt;
        double m_valueDouble;

    private:
        // 配置项记录的功能开关配置项的内部标识码
        static const int InternalIdentifyCode;
    };
}


#pragma warning( pop )
