#pragma once

#include "IEventArgs.h"

namespace JackC
{
    class JACKC_COMMON_EXPORT IStatusProgressBarChangeEventArgs : public IEventArgs
    {
        DEFINE_CAST_DERIVED(IStatusProgressBarChangeEventArgs, IEventArgs)

    public:
        virtual void SetEvent(IEvent* pEvent) = 0;

        virtual int GetCurrentIndex() const = 0;

        virtual void SetCurrentIndex(int value) = 0;

        virtual int GetTotalSize() const = 0;

        virtual void SetTotalSize(int value) = 0;

        virtual void SetCaption(const std::wstring &caption) = 0;

        virtual const std::wstring& GetCaption() const = 0;
    };
}