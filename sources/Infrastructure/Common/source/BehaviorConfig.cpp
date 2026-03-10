#include "BehaviorConfig.h"
#include "DevServiceHeaders.h"
#include "pugixml.h"
#include "JackCNew.h"
#include <sstream>
#include <iomanip>
#include "FileSystem.h"
#include "FilePath.h"
#include "JackCAssert.h"
#include "IBehaviorConfig.h"

using namespace JackC;


BehaviorConfigMgr * BehaviorConfigMgr::Get()
{
    static BehaviorConfigMgr s_instance;
    return &s_instance;
}

bool BehaviorConfigMgr::SetBehaviorConfigPath(const std::wstring& behaviorConfigPath)
{
    FileOwnerPtr fp = FileSystem::OpenFile(behaviorConfigPath, L"rb");
    JACKC_ASSERT(fp && "找不到BehaviorConfig配置文件");

    if (m_configFilePath == behaviorConfigPath)
    {
        return true;
    }

    m_configFilePath = behaviorConfigPath;
    return LoadConfigFile();
}

const std::vector<OwnerPtr<IConfigRecord>>& BehaviorConfigMgr::GetConfigRecords() const
{
    return m_configRecords;
}

const IConfigRecord* BehaviorConfigMgr::GetConfigRecord(const std::wstring& recordName) const
{
    for(int loop = 0; loop < (int)m_configRecords.size(); ++loop)
    {
        if(recordName == m_configRecords[loop]->GetName())
        {
            return m_configRecords[loop].get();
        }
    }
    return nullptr;
}

bool BehaviorConfigMgr::AddConfigRecord(OwnerPtr<IConfigRecord> opConfigRecord)
{
    if(GetConfigRecord(opConfigRecord->GetName()))
        return false;

    m_configRecords.push_back(TransferOwnership(opConfigRecord));
    return true;
}

bool BehaviorConfigMgr::IsValidProductType(const std::wstring & productType)
{
    return true;
}

bool BehaviorConfigMgr::UpdateConfigRecord(OwnerPtr<IConfigRecord> opConfigRecord)
{
    FOR_EACH(record, m_configRecords)
    {
        if (opConfigRecord->GetName() == record->GetName())
        {
            record = TransferOwnership(opConfigRecord);
            return true;
        }
    }
    return false;
}

std::wstring BehaviorConfigMgr::RedirectConfigFilePath( const std::wstring& configFilePath )
{
    std::wstring previousConfigFilePath = m_configFilePath;
    m_configFilePath = configFilePath;
    return previousConfigFilePath;
}

bool BehaviorConfigMgr::LoadConfigFile()
{
    pugi::xml_document xmlDoc;
    pugi::xml_parse_result result = xmlDoc.load_file(m_configFilePath.c_str());
    if (result.status != pugi::status_ok)
    {
        return false;
    }

    m_configRecords.clear();
    bool bSucceed = true;
    pugi::xpath_node_set nodeSet = xmlDoc.select_nodes(L"ConfigRoot/ConfigRecord");
    for(auto pChild = nodeSet.begin(); pChild != nodeSet.end(); ++pChild)
    {
        std::wstring attrType = pChild->node().attribute(L"type").as_string();
        std::wstring attrName = pChild->node().attribute(L"name").as_string();
        std::wstring attrValue = pChild->node().attribute(L"value").as_string();

        ConfigRecordType cfgRecordType = ConfigRecordType::InValid;
        for(int loop = static_cast<std::underlying_type<ConfigRecordType>::type>(ConfigRecordType::InValid);
                loop < static_cast<std::underlying_type<ConfigRecordType>::type>(ConfigRecordType::Num_Of_Types);
                ++loop)
        {
            if(IConfigRecord::GetConfigRecordTypeNames()[loop] == attrType)
            {
                cfgRecordType = (ConfigRecordType)loop;
                break;
            }
        }

        switch(cfgRecordType)
        {
        case ConfigRecordType::Double:
            {
                std::wistringstream os(attrValue);
                double valueDouble = 0;
                os >> valueDouble;
                OwnerPtr<IConfigRecord> opAcr = IConfigRecord::Create(attrName, valueDouble);
                AddConfigRecord(TransferOwnership(opAcr));
            }
            break;
        case ConfigRecordType::Int:
            {
                std::wistringstream os(attrValue);
                int valueInt = 0;
                os >> valueInt;
                OwnerPtr<IConfigRecord> opAcr = IConfigRecord::Create(attrName, valueInt);
                AddConfigRecord(TransferOwnership(opAcr));
            }
            break;
        case ConfigRecordType::String:
            {
                OwnerPtr<IConfigRecord> opAcr 
                    = IConfigRecord::Create(attrName, attrValue);
                AddConfigRecord(TransferOwnership(opAcr));
            }
            break;
        default:
            {
                DBG_WARN(L"遇到了无法识别的配置类型。", L"jack.chen", L"2025-07-19");
                return false;
            }
        }
    }
    return bSucceed;
}

bool BehaviorConfigMgr::SaveConfigFile() const
{
    pugi::xml_document xmlDoc;
    xmlDoc.append_child(L"ConfigRoot");
    for (int loop = 0; loop < (int)m_configRecords.size(); ++loop)
    {
        const IConfigRecord* pRecord = m_configRecords[loop].get();

        pugi::xml_node recordNode = xmlDoc.first_child().append_child(L"ConfigRecord");
        recordNode.append_attribute(L"type") 
            = IConfigRecord::GetConfigRecordTypeNames()[static_cast<std::underlying_type<ConfigRecordType>::type>(pRecord->GetType())].c_str();
        recordNode.append_attribute(L"name") = pRecord->GetName().c_str();
        recordNode.append_attribute(L"value") = pRecord->GetStringOfValue().c_str();
    }

    return xmlDoc.save_file(m_configFilePath.c_str());
}
