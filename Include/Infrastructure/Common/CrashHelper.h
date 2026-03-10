#pragma once
#include "JackCDevService.h"

namespace JackC
{
    //崩溃工具类
    class JACKC_COMMON_EXPORT CrashHelper
    {
    public:
        static CrashHelper* GetInstance();

        void Crash(const std::wstring &expression, 
            const std::wstring &fileName, 
            int line, 
            const std::wstring &function,
            const std::wstring &crashTips) const;

    private:
        CrashHelper() {}
        virtual ~CrashHelper() {}
    };
}
