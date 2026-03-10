#pragma once
#include "JackCDevService.h"
#include "Color.h"


namespace JackC
{
    class JACKC_COMMON_EXPORT ColorUtils
    {
    public:

        //Color 转为字符串（格式为 "R,G,B")
        static std::wstring GetWStringOfColor(const Color& color);

        static std::string GetStringOfColor(const Color& color);

        //从宽字符串转为Color
        static bool GetColorFromWString(const std::wstring& input, Color& color);

        //新UI框架使用,从字符串取出颜色值并验证颜色值是否合法
        static bool CheckIsValidColor(const std::wstring& input, Color& color);
    };
    
}
