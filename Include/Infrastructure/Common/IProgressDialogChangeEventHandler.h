#pragma once

#include "JackCDevService.h"
#include "IObject.h"

namespace JackC
{
    class IProgressDialogChangeEventArgs;

    class JACKC_COMMON_EXPORT IProgressDialogChangeEventHandler : public IObject
    {
        DEFINE_CAST_DERIVED(IProgressDialogChangeEventHandler, IObject)

    public:
        virtual ~IProgressDialogChangeEventHandler() {}

        virtual void On(IProgressDialogChangeEventArgs *pArgs) = 0;
    };
}