#pragma once

#include "JackCDevService.h"
#include "IEvent.h"
#include "DebugMessage.h"

namespace JackC
{
    class IProgressDialogChangeEventHandler;
    class IProgressDialogChangeEventArgs;
    class IAppWindow;

    class JACKC_COMMON_EXPORT IProgressDialogChangeEvent : public IEvent
    {
        DEFINE_CAST_DERIVED(IProgressDialogChangeEvent, IEvent)

    public:

        virtual ~IProgressDialogChangeEvent(){}

        virtual bool Add(IProgressDialogChangeEventHandler *pHandler) = 0;

        virtual bool Delete(IProgressDialogChangeEventHandler *pHandler) = 0;

        virtual IAppWindow *GetSender() const = 0;

        virtual void Emit(IProgressDialogChangeEventArgs* pArgs) = 0;

    };

}