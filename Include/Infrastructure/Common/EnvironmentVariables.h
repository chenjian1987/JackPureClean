#pragma once

#include "JackCDevService.h"

#ifdef _WINDOWS
#include <Windows.h>
#endif

namespace JackC
{
    namespace EnvironmentVariables
    {
        FORCE_INLINE std::wstring GetVariable(const std::wstring& name)
        {
            std::wstring res;

#ifdef _WINDOWS
            if (name.empty())
                return res;

            DWORD dwRet = ::GetEnvironmentVariableW(name.c_str(), nullptr, 0);
            if (dwRet != 0)
            {
                LPTSTR pszOldVal = (LPTSTR)malloc(dwRet * sizeof(TCHAR));
                ::GetEnvironmentVariableW(name.c_str(), pszOldVal, dwRet);
                res = pszOldVal;
                free(pszOldVal);
            }
#endif // _WINDOWS

            return res;
        }

        FORCE_INLINE void SetVariable(const std::wstring& name, const std::wstring& value)
        {
            if (name.empty())
                return;

#ifdef _WINDOWS
            SetEnvironmentVariableW(name.c_str(), value.c_str());
#endif // _WINDOWS
        }

        FORCE_INLINE std::wstring AddPath(const std::wstring& folderPath)
        {
            std::wstring resultPath = folderPath;

#ifdef _WINDOWS
            const std::wstring varName = L"Path";
            const std::wstring separator = L";";
            const std::wstring originalPath = GetVariable(varName);
            if (originalPath.size() > 0)
            {
                resultPath += separator;
                resultPath += originalPath;
            }
#endif // _WINDOWS

            return resultPath;
        }
    }

    class JACKC_COMMON_EXPORT EnvironmentVariableSetter
    {
    public:
        EnvironmentVariableSetter(const std::wstring& name, const std::wstring& value) : m_name(name)
        {
            if (m_name.empty())
                return;

            m_originalValue = EnvironmentVariables::GetVariable(m_name);
            EnvironmentVariables::SetVariable(m_name.c_str(), value.c_str());
        }
        virtual ~EnvironmentVariableSetter()
        {
            if (m_name.empty())
                return;

            EnvironmentVariables::SetVariable(m_name.c_str(), m_originalValue.c_str());
        }

    private:
        std::wstring m_name;
        std::wstring m_originalValue;
    };
}
