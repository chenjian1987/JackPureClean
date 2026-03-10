#pragma once

#include "AssertHelper.h"
#include "JackCDevService.h"
#include "BasicConfigUtils.h"

#ifdef _WINDOWS
#define JACKC_ASSERT(EXPRESSION) \
    if(!(EXPRESSION)) \
    { \
        if(JackC::BasicConfigUtils::DebugMessageEnabled()) \
            JackC::AssertHelper::GetInstance()->Assert(_CRT_WIDE(#EXPRESSION), _CRT_WIDE(__FILE__), __LINE__);\
        else \
            JackC::DumpAssertToLog(#EXPRESSION, __FILE__, __LINE__, __FUNCTION__); \
    }

#define JACKC_DBG_WARN(MESSAGE, FILENAME, LINE) \
    { \
        if (JackC::BasicConfigUtils::DebugMessageEnabled()) \
            JackC::AssertHelper::GetInstance()->Assert(MESSAGE, FILENAME, LINE);\
        else \
            JackC::DumpDbgWarnToLog(MESSAGE, FILENAME, LINE); \
    }
#else
#define JACKC_ASSERT(EXPRESSION) \
    if(!(EXPRESSION)) \
    { \
        if (JackC::BasicConfigUtils::DebugMessageEnabled()) \
            jackc_assert_impl(EXPRESSION); \
        else \
            JackC::DumpAssertToLog(#EXPRESSION, __FILE__, __LINE__, __FUNCTION__); \
    }
#endif

namespace JackC
{
    void JACKC_COMMON_EXPORT DumpAssertToLog(const std::string& expression, const std::string& file, int line, const std::string& function);
    void JACKC_COMMON_EXPORT DumpDbgWarnToLog(const std::wstring &message, const std::wstring &file, int line);
}