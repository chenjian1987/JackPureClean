#pragma once

#include "JackCDevService.h"

namespace JackC
{
    class JACKC_COMMON_EXPORT BasicConfigUtils
    {
    public:
        static bool DebugMessageEnabled();
        static bool IsRunAsService();
        static bool LogEnabled();
        static bool DumpEnabled();
        static bool IsAppInHouse();

    private:
        static bool CampareConfigValue(const std::wstring &key, int target);
        static bool CampareConfigValue(const std::wstring &key, const std::wstring &target);
    };
} // namespace JackC
