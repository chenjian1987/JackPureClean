#include "ConfigRecord.h"
#include "DevServiceHeaders.h"
#include "pugixml.h"
#include "JackCNew.h"
#include <sstream>
#include <iomanip>
#include "FileSystem.h"
#include "FilePath.h"

#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"

using namespace JackC;

const int ConfigRecord::InternalIdentifyCode = 1;

ConfigRecord::~ConfigRecord()
{
}

ConfigRecord::ConfigRecord(
    ConfigRecordType type,
    const std::wstring& name,
    const std::wstring& stringValue)
    : m_type(type)
    , m_name(name)
    , m_valueString(stringValue)
    , m_valueInt(0)
    , m_valueDouble(0)
{
}

ConfigRecord::ConfigRecord(
    ConfigRecordType type,
    const std::wstring& name,
    int intValue)
    : m_type(type)
    , m_name(name)
    , m_valueString()
    , m_valueInt(intValue)
    , m_valueDouble(0)
{
}

ConfigRecord::ConfigRecord(
    ConfigRecordType type,
    const std::wstring& name,
    double doubleValue)
    : m_type(type)
    , m_name(name)
    , m_valueString()
    , m_valueInt(0)
    , m_valueDouble(doubleValue)
{
}

ConfigRecord::ConfigRecord(const ConfigRecord& other)
    : m_type(other.m_type)
    , m_name(other.m_name)
    , m_valueString(other.m_valueString)
    , m_valueInt(other.m_valueInt)
    , m_valueDouble(other.m_valueDouble)
{
}

ConfigRecord& ConfigRecord::operator= (const ConfigRecord& other)
{
    if(this == &other)
    {
        return *this;
    }

    m_type = other.m_type;
    m_name = other.m_name;
    m_valueString = other.m_valueString;
    m_valueInt = other.m_valueInt;
    m_valueDouble = other.m_valueDouble;

    return *this;
}

OwnerPtr<JackC::IConfigRecord> ConfigRecord::Clone() const
{
    return NEW_AS_OWNER_PTR(ConfigRecord, *this);
}

std::wstring ConfigRecord::GetValueAsString() const
{
    DBG_WARN_UNLESS(GetType() == ConfigRecordType::String, L"类型不匹配", L"jack.chen", L"2025-07-19");
    return m_valueString;
}

int ConfigRecord::GetValueAsInt() const
{
    DBG_WARN_UNLESS(GetType() == ConfigRecordType::Int, L"类型不匹配", L"jack.chen", L"2025-07-19");
    return m_valueInt;
}

double ConfigRecord::GetValueAsDouble() const
{
    DBG_WARN_UNLESS(GetType() == ConfigRecordType::Double, L"类型不匹配", L"jack.chen", L"2025-07-19");
    return m_valueDouble;
}

std::wstring ConfigRecord::GetStringOfValue() const
{
    std::wostringstream os;
    std::wstring stringForXmlOutput;

    switch(GetType())
    {
    case ConfigRecordType::Int:
        os << GetValueAsInt();
        stringForXmlOutput.assign(os.str());
        break;
   case ConfigRecordType::Double:
        os << GetValueAsDouble();
        stringForXmlOutput.assign(os.str());
        break;
    case ConfigRecordType::String:
        stringForXmlOutput.assign(GetValueAsString());
        break;
    default:
        DBG_WARN(L"无法识别的类型", L"jack.chen", L"2015-07-15");
        return L"";
    }
    return stringForXmlOutput;
}

bool JackC::ConfigRecord::IsInternalIdentifyCodeValue() const
{
    return InternalIdentifyCode == m_valueInt;
}

