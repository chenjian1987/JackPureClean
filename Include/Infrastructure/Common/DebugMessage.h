#pragma once

#include "JackCDevService.h"

#pragma warning( push )
#pragma warning( disable : 4251 )

namespace JackC
{
    //方便开发人员弹出可见的对话框，以传递程序运行过程中可能的问题,
    class JACKC_COMMON_EXPORT DebugMessageMgr
    {
    private:
        DebugMessageMgr(void);

    public:
        virtual ~DebugMessageMgr(void);

    public:
        static const DebugMessageMgr* GetDebugMessageMgr();
        static DebugMessageMgr* GetDebugMessageMgrFW();

    public:
        void ShowDebugWarn(
            const std::string& fileName, 
            int lineNumber, 
            const std::string& functionName, 
            const std::wstring& text, 
            const std::wstring& name, 
            const std::wstring& date) const;
        
        void RegisterAsPreShowListener(std::function<void()> listener);
        void RegisterAsPostShowListener(std::function<void()> listener);
        void SetWillQuitApplication(bool bWillQuitApplication);
        bool IsShowingDebugWarn() const;
    };
}

#define DBG_WARN(text, name, date) \
    JackC::DebugMessageMgr::GetDebugMessageMgr()->ShowDebugWarn(__FILE__, __LINE__, __FUNCTION__, text, name, date);

#define DBG_WARN_UNLESS(condition, text, name, date) \
    if(!(condition)) \
    { \
        JackC::DebugMessageMgr::GetDebugMessageMgr()->ShowDebugWarn(__FILE__, __LINE__, __FUNCTION__, text, name, date); \
    }

#define DBG_WARN_AND_RETURN_UNLESS(condition, returnValue, text, name, date) \
    if(!(condition)) \
    { \
        JackC::DebugMessageMgr::GetDebugMessageMgr()->ShowDebugWarn(__FILE__, __LINE__, __FUNCTION__, text, name, date); \
        return (returnValue); \
    }

#define DBG_WARN_AND_RETURN_VOID_UNLESS(condition, text, name, date) \
    if(!(condition)) \
    { \
        JackC::DebugMessageMgr::GetDebugMessageMgr()->ShowDebugWarn(__FILE__, __LINE__, __FUNCTION__, text, name, date); \
        return; \
    }

#define DBG_WARN_AND_RETURN_NULLPTR_UNLESS(condition, text, name, date) \
    if(!(condition)) \
    { \
        JackC::DebugMessageMgr::GetDebugMessageMgr()->ShowDebugWarn(__FILE__, __LINE__, __FUNCTION__, text, name, date); \
        return nullptr; \
    }

#define DBG_WARN_AND_RETURN_FALSE_UNLESS(condition, text, name, date) \
    if(!(condition)) \
    { \
        JackC::DebugMessageMgr::GetDebugMessageMgr()->ShowDebugWarn(__FILE__, __LINE__, __FUNCTION__, text, name, date); \
        return false; \
    }

#define DBG_WARN_AND_CONTINUE_UNLESS(condition, text, name, date) \
    if(!(condition)) \
    { \
        JackC::DebugMessageMgr::GetDebugMessageMgr()->ShowDebugWarn(__FILE__, __LINE__, __FUNCTION__, text, name, date); \
        continue; \
    }

#define DBG_WARN_AND_BREAK_UNLESS(condition, text, name, date) \
    if(!(condition)) \
    { \
        JackC::DebugMessageMgr::GetDebugMessageMgr()->ShowDebugWarn(__FILE__, __LINE__, __FUNCTION__, text, name, date); \
        break; \
    }

#pragma warning( pop )
