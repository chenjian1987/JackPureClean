#pragma once

#include "JackCDevService.h"
#include "WeakReferenceable.h"
#include "JackCGuid.h"

namespace JackC
{
    class IProgressDialogChangeEvent;

    class JACKC_COMMON_EXPORT IProgressDialogDefinition : public WeakReferenceable
    {
    public:
        virtual ~IProgressDialogDefinition(){}

        virtual JackCGuid GetGuid() = 0;

        virtual void SetRange(int minimum, int maximum) = 0;
        virtual int GetMinimum() const = 0;
        virtual int GetMaximum() const = 0;

        virtual void SetValue(int progress) = 0;
        virtual int GetValue() const = 0;

        virtual void AutoIncrease(int step = 1) = 0;

        virtual void SetTitle(const std::wstring& text) = 0;
        virtual std::wstring GetTitle() const = 0;

        virtual void SetCancelButtonText(const std::wstring& text) = 0;
        virtual std::wstring GetCancelButtonText() const = 0;

        virtual void SetMinimumDuration(int ms) = 0;
        virtual int GetMinimumDuration() const = 0;

        virtual void SetAutoReset(bool reset) = 0;
        virtual bool GetAutoReset(bool reset) const = 0;

        virtual void SetCanceled(bool value) = 0;
        virtual bool GetCanceled() const = 0;

        virtual void SetAutoClose(bool close) = 0;
        virtual bool GetAutoClose() const = 0;

        virtual IProgressDialogChangeEvent* GetProgressDialogChangeEvent() = 0;
    };
}

