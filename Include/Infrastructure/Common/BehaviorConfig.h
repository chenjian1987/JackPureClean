#pragma once

#include "JackCDevService.h"
#include "IConfigRecord.h"

namespace JackC
{
    // Singleton类，管理程序行为配置文件
    class BehaviorConfigMgr
    {
    public:
        static BehaviorConfigMgr* Get();

        // 第一次使用前，用户需指定behavior_config的路径
        bool SetBehaviorConfigPath(const std::wstring& behaviorConfigPath);

        const IConfigRecord* GetConfigRecord(const std::wstring& recordName) const;

        bool SaveConfigFile() const;
        static bool IsValidProductType(const std::wstring& productType);

        // 添加一条配置，如果同名配置已存在则跳过
        bool AddConfigRecord(OwnerPtr<IConfigRecord> opConfigRecord);
        
        // 修改一条配置，不存在则跳过（可提供API）
        bool UpdateConfigRecord(OwnerPtr<IConfigRecord> opConfigRecord);

        //日志模块需要用到
        std::wstring RedirectConfigFilePath(const std::wstring& configFilePath);

    private:
        const std::vector<OwnerPtr<IConfigRecord>>& GetConfigRecords() const;
        bool LoadConfigFile();

    private:
        std::vector<OwnerPtr<IConfigRecord>> m_configRecords;
        std::wstring                         m_configFilePath;
    };
}

#define FIND_BEHAVIOR_CONFIG(name) \
    (JackC::BehaviorConfigMgr::Get()->GetConfigRecord(name))

#define LOOKUP_BEHAVIOR_CONFIG_INT_VALUE(name) \
    (JackC::BehaviorConfigMgr::Get()->GetConfigRecord(name) == nullptr ? 0 : JackC::BehaviorConfigMgr::Get()->GetConfigRecord(name)->GetValueAsInt())

#define LOOKUP_BEHAVIOR_CONFIG_DOUBLE_VALUE(name) \
    (JackC::BehaviorConfigMgr::Get()->GetConfigRecord(name) == nullptr ? 0.0 : JackC::BehaviorConfigMgr::Get()->GetConfigRecord(name)->GetValueAsDouble())

#define LOOKUP_BEHAVIOR_CONFIG_STRING_VALUE(name) \
    (JackC::BehaviorConfigMgr::Get()->GetConfigRecord(name) == nullptr ? L"" : JackC::BehaviorConfigMgr::Get()->GetConfigRecord(name)->GetValueAsString())
