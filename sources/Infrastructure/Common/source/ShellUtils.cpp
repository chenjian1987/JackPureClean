#include "windows.h"
#include "ShellUtils.h"
#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"

using namespace JackC;

//执行windows命令行
void ShellUtils::ExecuteShellCmd(const std::wstring& shellExecuteFilePath, const std::wstring& shellExecuteParameterString)
{
    SHELLEXECUTEINFO  ShExecInfo = { 0 };
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd = NULL;
    ShExecInfo.lpVerb = LPCWSTR(L"open");
    ShExecInfo.lpFile = LPCWSTR(shellExecuteFilePath.c_str());
    ShExecInfo.lpParameters = LPCWSTR(shellExecuteParameterString.c_str());
    ShExecInfo.lpDirectory = NULL;
    ShExecInfo.nShow = SW_HIDE;
    ShExecInfo.hInstApp = NULL;
    ShellExecuteEx(&ShExecInfo);

    DWORD dwExitCode;
    GetExitCodeProcess(ShExecInfo.hProcess, &dwExitCode);
    while (dwExitCode == STILL_ACTIVE)
    {
        Sleep((DWORD)5);
        GetExitCodeProcess(ShExecInfo.hProcess, &dwExitCode);
    }
    CloseHandle(ShExecInfo.hProcess);
}