#include "StatusProgressBarChangeEventArgs.h"

#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"

using namespace JackC;

void StatusProgressBarChangeEventArgs::SetEvent(IEvent* pEvent)
{
    m_pEvent = pEvent;
}

int StatusProgressBarChangeEventArgs::GetCurrentIndex() const
{
    return m_currentIndex;
}

void StatusProgressBarChangeEventArgs::SetCurrentIndex(int value)
{
    m_currentIndex = value;
}

int StatusProgressBarChangeEventArgs::GetTotalSize() const
{
    return m_totalSize;
}

void StatusProgressBarChangeEventArgs::SetTotalSize(int value)
{
    m_totalSize = value;
}

const IEvent* StatusProgressBarChangeEventArgs::GetEvent() const
{
    return m_pEvent;
}

void StatusProgressBarChangeEventArgs::SetCaption(const std::wstring& caption)
{
    m_caption = caption;
}

const std::wstring& StatusProgressBarChangeEventArgs::GetCaption() const
{
    return m_caption;
}
