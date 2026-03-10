#pragma once

#include "IEventArgs.h"
#include "JackCGuid.h"

namespace JackC
{
    class IProgressDialogDefinition;

    class JACKC_COMMON_EXPORT IProgressDialogChangeEventArgs : public IEventArgs
    {
        DEFINE_CAST_DERIVED(IProgressDialogChangeEventArgs, IEventArgs)

    public:
        virtual ~IProgressDialogChangeEventArgs() {}

        virtual void SetEvent(IEvent* pEvent) = 0;

        virtual void SetProgressDialogDefinitionGuid(const JackCGuid & guid) = 0;

        virtual JackCGuid GetProgressDialogDefinitionGuid() = 0;
    };
}