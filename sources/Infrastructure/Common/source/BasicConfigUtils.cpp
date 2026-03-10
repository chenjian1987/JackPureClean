#include "BasicConfigUtils.h"
#include "IConfigRecord.h"
#include "IBehaviorConfig.h"
#include "LogManager.h"

using namespace JackC;

namespace
{
    const int c_TargetIntValue = 1;
    const std::wstring c_TargetInHouseValue = L"8A6B3148-68DB-41E0-96DC-7BBD52209E7F";
}

bool BasicConfigUtils::DebugMessageEnabled() 
{
    if (!LogManager::IsReady())
    {
        return false;
    }
    // 内部版本始终显示DebugMessage
    if (IsAppInHouse())
    {
        return true;
    }
    return CampareConfigValue(BehaviorConfigRecordNames::EnableDebugMessage, c_TargetIntValue);
}

bool BasicConfigUtils::IsRunAsService() 
{
    return CampareConfigValue(BehaviorConfigRecordNames::RunAsService, c_TargetIntValue);
}

bool BasicConfigUtils::LogEnabled() 
{
    return CampareConfigValue(BehaviorConfigRecordNames::EnableLog, c_TargetIntValue);
}

bool BasicConfigUtils::DumpEnabled()
{
    return CampareConfigValue(BehaviorConfigRecordNames::EnableDump, c_TargetIntValue);
}

bool BasicConfigUtils::IsAppInHouse() 
{
    return CampareConfigValue(BehaviorConfigRecordNames::ProductStage, c_TargetInHouseValue);
}

bool BasicConfigUtils::CampareConfigValue(const std::wstring &key, int target) 
{
    const IConfigRecord *pConfigRecord = IBehaviorConfig::GetConfigRecord(key);
    if (pConfigRecord != nullptr)
    {
        return pConfigRecord->GetValueAsInt() == target;
    }

    return false;
}

bool BasicConfigUtils::CampareConfigValue(const std::wstring &key, const std::wstring &target) 
{
    const IConfigRecord *pConfigRecord = IBehaviorConfig::GetConfigRecord(key);
    if (pConfigRecord != nullptr)
    {
        return pConfigRecord->GetStringOfValue() == target;
    }

    return false;
}
