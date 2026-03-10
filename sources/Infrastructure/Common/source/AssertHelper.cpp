#include "AssertHelper.h"
#include <sstream>
#include "LogManager.h"
#include "IConfigRecord.h"
#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"

using namespace JackC;




AssertHelper* AssertHelper::GetInstance()
{
    static  AssertHelper s_assertHelper;
    return  &s_assertHelper;
}



void AssertHelper::RegisterAsPreShowListener(std::function<void()> listener)
{
    m_preShowListeners.push_back(listener);
}




void AssertHelper::Assert(const std::wstring &text, const std::wstring &fileName, int line) const
{
    std::wostringstream woss;
    woss << L"File: " << fileName.c_str() << L"\n\rLine: " << line << L"\n\r\n\r";
    woss << L"Assert:" << text;

#if defined(__clang__) &&defined(__linux__)
    // 暂不同时输出到Log
#else
    // 先输出到日志中
    LogManager::CreateOrGetForWrite()->WriteLine(LogTypeOfLogManager::DEBUG_ERROR, woss.str(), true);
#endif

    std::for_each(m_preShowListeners.begin(), m_preShowListeners.end(), [](std::function<void()> listener) 
    {
        listener();
    });
#ifdef _WINDOWS
    _wassert(text.c_str(), fileName.c_str(), line);
#endif
}
