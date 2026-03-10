#pragma once

#include "JackCDevService.h"

namespace JackC
{
    /// DebugWarn处理接口
    /// 若没有通过SwitchTo接口进行切换， 则默认为Assert操作。
    /// 同时平台提供了Dialog样式的处理接口,
    /// 可通过IDebugWarningDialogDisplayer::Create() 创建出来。
    class JACKC_COMMON_EXPORT IDebugWarningDisplayer
    {
    public:
        //触发警告时的显示接口
        virtual bool Display(const std::string& fileName,
            int lineNumber,
            const std::string& functionName,
            const std::wstring& text,
            const std::wstring& name,
            const std::wstring& date) = 0;

        //多线程下DebugWarn是否弹出Assert
        virtual bool ShouldAssertInMultithread() const = 0;

    public:
        //获取当前的调试警告显示处理对象
        static IDebugWarningDisplayer* Get(); 

        //切换当前的调试警告处理对象
        static bool SwitchTo(OwnerPtr<IDebugWarningDisplayer> debugWarningDisplayer);

        //创建一个默认的Assert显示对象
        static OwnerPtr<IDebugWarningDisplayer> CreateAssertDisplayer();
    };
}

