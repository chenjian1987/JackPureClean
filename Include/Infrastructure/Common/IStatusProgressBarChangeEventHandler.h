#pragma once

#include "JackCDevService.h"
#include "IObject.h"

namespace JackC
{
    class IStatusProgressBarChangeEventArgs;

    class JACKC_COMMON_EXPORT IStatusProgressBarChangeEventHandler : public IObject
    {
        DEFINE_CAST_DERIVED(IStatusProgressBarChangeEventHandler, IObject)

    public:
        virtual void On(IStatusProgressBarChangeEventArgs *pArgs) = 0;
    };
}