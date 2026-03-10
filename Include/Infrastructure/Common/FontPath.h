#pragma once

#include "JackCDevService.h"

namespace JackC
{
    class JACKC_COMMON_EXPORT FontPath
    {
    public:
        // 获取系统文件夹下(通常是C:\Windows\Fonts\)给定字体的路径
        // 如果找不到则返回空字符串
        static std::wstring GetSystemFontPath(const std::wstring &fontName);
    };
}

