#include "IStatusProgressBarChangeEvent.h"
#include "IStatusProgressBarChangeEventHandler.h"

#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"

using namespace JackC;

IStatusProgressBarChangeEvent *IStatusProgressBarChangeEvent::ms_pEvent = nullptr;

IStatusProgressBarChangeEvent* IStatusProgressBarChangeEvent::Get()
{
    DBG_WARN_AND_RETURN_NULLPTR_UNLESS(ms_pEvent, L"IStatusProgressBarChangeEvent 没有初始化", L"jack.chen", L"2025/9/26");

    return ms_pEvent;
};
