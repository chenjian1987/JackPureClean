#pragma once

#include "JackCDevService.h"
#include "IEvent.h"
#include "DebugMessage.h"

namespace JackC
{
    class IStatusProgressBarChangeEventHandler;
    class IStatusProgressBarChangeEventArgs;
    class IAppWindow;

    class JACKC_COMMON_EXPORT IStatusProgressBarChangeEvent : public IEvent
    {
        DEFINE_CAST_DERIVED(IStatusProgressBarChangeEvent, IEvent)

    public:

        static IStatusProgressBarChangeEvent* Get();


        virtual ~IStatusProgressBarChangeEvent(){}

        virtual bool Add(IStatusProgressBarChangeEventHandler *pHandler) = 0;

        virtual bool Delete(IStatusProgressBarChangeEventHandler *pHandler) = 0;

        virtual IAppWindow *GetSender() const = 0;

        virtual void Emit(IStatusProgressBarChangeEventArgs* pArgs) = 0;

    protected:
        static IStatusProgressBarChangeEvent *ms_pEvent;
    };

}