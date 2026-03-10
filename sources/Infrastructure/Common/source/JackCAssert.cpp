#include "JackCAssert.h"
#include "JACKC_ASSERT.h"
#include "LogManager.h"
#include <sstream>
#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"

void JackC::DumpAssertToLog(const std::string& expression, 
    const std::string& file, 
    int line, 
    const std::string& function)
{
    std::wostringstream woss;
    woss << L"Assert: " << expression.c_str() 
        << L", File: "  << file.c_str() 
        << L", Line: " << line 
        << L", Function: " << function.c_str()
        << L".";
    LogManager::CreateOrGetForWrite()->WriteLine(LogTypeOfLogManager::DEBUG_ERROR, woss.str(), true);
}

void JackC::DumpDbgWarnToLog(
    const std::wstring &message, const std::wstring &file, int line)
{
    std::wostringstream woss;
    woss << L"Warn: " << message << L", File: " << file << L", Line: " << line << L".";
    LogManager::CreateOrGetForWrite()->WriteLine(LogTypeOfLogManager::DEBUG_WARN, woss.str(), true);
}
