#pragma once

#include "JackCDevService.h"


namespace JackC
{
    class JACKC_COMMON_EXPORT ShellUtils
    {
    public:
        static void ExecuteShellCmd(const std::wstring& shellExecuteFilePath, const std::wstring& shellExecuteParameterString);
    };
}
