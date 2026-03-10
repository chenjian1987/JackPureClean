#include "IProgressDialogFactory.h"
#include "FileSystem.h"
#include "DebugMessage.h"
#include "IProgressDialogChangeEventArgs.h"
#include "IProgressDialogChangeEvent.h"
#include "IProgressDialogChangeEventHandler.h"
#include "IProgressDialogDefinition.h"

using namespace JackC;

IProgressDialogFactory* IProgressDialogFactory::ms_pInstance = nullptr;

IProgressDialogFactory* IProgressDialogFactory::Get()
{
    if (ms_pInstance == nullptr)
    {
#ifdef __linux__
        FileSystem::ModuleHandle handle = FileSystem::LoadModule(L"libJackCGuiMainFrame.so");
        DBG_WARN_AND_RETURN_NULLPTR_UNLESS(handle, L"libJackCGuiMainFrame.so加载失败", L"jack.chen", L"2025-09-25");
#else
        FileSystem::ModuleHandle handle = FileSystem::LoadModule(L"JackCGuiMainFrame.dll");
        DBG_WARN_AND_RETURN_NULLPTR_UNLESS(handle, L"JackCGuiMainFrame没有加载", L"jack.chen", L"2024-02-20");
#endif
    }

    DBG_WARN_AND_RETURN_NULLPTR_UNLESS(ms_pInstance != nullptr, L"JackCCommonEdit没有创建对象", L"jack.chen", L"2025-08-25");
    return ms_pInstance;
}
