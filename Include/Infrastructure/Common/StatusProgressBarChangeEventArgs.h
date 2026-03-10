#pragma once

#include "IStatusProgressBarChangeEventArgs.h"


namespace JackC
{
    class ElementId;
    class IDocument;

    class JACKC_COMMON_EXPORT StatusProgressBarChangeEventArgs : public IStatusProgressBarChangeEventArgs
    {
        DEFINE_CAST_DERIVED(StatusProgressBarChangeEventArgs, IStatusProgressBarChangeEventArgs)

    public:
        virtual void SetEvent(IEvent* pEvent) override;

        virtual int GetCurrentIndex() const override;

        virtual void SetCurrentIndex(int value) override;

        virtual int GetTotalSize() const override;

        virtual void SetTotalSize(int value) override;

        virtual const IEvent* GetEvent() const override;

        virtual void SetCaption(const std::wstring& caption) override;

        virtual const std::wstring& GetCaption() const override;

    private:
        int m_currentIndex;
        int m_totalSize;
        std::wstring m_caption;
        IEvent* m_pEvent;
    };
}