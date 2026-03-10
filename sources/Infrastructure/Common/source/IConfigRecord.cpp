#include "IConfigRecord.h"
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

OwnerPtr<IConfigRecord> IConfigRecord::Create(const std::wstring& name, const std::wstring& stringValue)
{
    return NEW_AS_OWNER_PTR(ConfigRecord, ConfigRecordType::String, name, stringValue);
}

OwnerPtr<IConfigRecord> IConfigRecord::Create(const std::wstring& name, double doubleValue)
{
    return NEW_AS_OWNER_PTR(ConfigRecord, ConfigRecordType::Double, name, doubleValue);
}

OwnerPtr<IConfigRecord> IConfigRecord::Create(const std::wstring& name, int intValue)
{
    return NEW_AS_OWNER_PTR(ConfigRecord, ConfigRecordType::Int, name, intValue);
}

const std::vector<std::wstring>& IConfigRecord::GetConfigRecordTypeNames()
{
    static std::vector<std::wstring> s_configRecordTypeNames(static_cast<std::underlying_type<ConfigRecordType>::type>(ConfigRecordType::Num_Of_Types));

    // 注意：若要添加新项，请同步添加对应字符串至s_configRecordTypeNames
    s_configRecordTypeNames[static_cast<std::underlying_type<ConfigRecordType>::type>(ConfigRecordType::InValid)] = L"invalid";
    s_configRecordTypeNames[static_cast<std::underlying_type<ConfigRecordType>::type>(ConfigRecordType::String)] = L"string";
    s_configRecordTypeNames[static_cast<std::underlying_type<ConfigRecordType>::type>(ConfigRecordType::Int)] = L"int";
    s_configRecordTypeNames[static_cast<std::underlying_type<ConfigRecordType>::type>(ConfigRecordType::Double)] = L"double";

    return s_configRecordTypeNames;
}
