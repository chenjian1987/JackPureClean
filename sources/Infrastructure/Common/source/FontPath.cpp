#include "FontPath.h"
#include "FileSystem.h"
#include "FilePath.h"
#ifdef _WINDOWS
#include <ShlObj.h>
#endif
#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"
using namespace JackC;

std::wstring FontPath::GetSystemFontPath(const std::wstring& fontName)
{
    std::wstring fontPath;
#ifdef _WINDOWS
    wchar_t systemFontsPath[MAX_PATH];
    HRESULT result = SHGetFolderPathW(NULL, CSIDL_FONTS, NULL, SHGFP_TYPE_CURRENT, systemFontsPath);
    if (result != S_OK)
    {
        return fontPath;
    }   

    fontPath = FilePath::Combine(systemFontsPath, fontName);
    FileInfo fileInfo(fontPath);
    if (fileInfo.Exists())
    {
        return fontPath;
    }

    return L"";
#else
    return fontPath;
#endif
}
