#pragma once

#include <list>
#include "WeakPtr.h"

namespace JackC
{
    template <typename BaseEvent, typename Handler, typename Args>
    class EventNoSender :public BaseEvent
    {
    public:
        EventNoSender(const std::wstring & name) :m_name(name) {}
        virtual ~EventNoSender()
        {
            m_wpHandlers.clear();
        }

        std::wstring GetName() const override
        {
            return m_name;
        };

        bool Delete(Handler * pHandler) override
        {
            if (!pHandler)
            {
                return false;
            }

            WeakPtr<Handler> wpHandler(pHandler);
            auto it = std::find(m_wpHandlers.begin(), m_wpHandlers.end(), wpHandler);
            if (it != m_wpHandlers.end())
            {
                // 防止Event::Emit的时候调用Delete，Erase会导致当前迭代器失效而崩溃
                it->Reset();
                return true;
            }

            return false;
        }

        int GetHandlerCount() const
        {
            return (int)m_wpHandlers.size();
        }

        bool Add(Handler * pHandler) override
        {
            if (!pHandler)
            {
                return false;
            }

            WeakPtr<Handler> wpHandler(pHandler);
            if (std::find(m_wpHandlers.begin(), m_wpHandlers.end(), wpHandler) == m_wpHandlers.end())
            {
                m_wpHandlers.emplace_back(wpHandler);
                return true;
            }

            return false;
        }

        void Emit(Args * pArgs)
        {
            if (pArgs)
            {
                pArgs->SetEvent(this);
            }

            for (auto pos = m_wpHandlers.begin(); pos != m_wpHandlers.end(); )
            {
                Handler * pHandler = (*pos).Get();
                if (pHandler != nullptr)
                {
                    pHandler->On(pArgs);
                    ++pos;
                }
                else
                {
                    pos = m_wpHandlers.erase(pos);
                }
            }
        }
    private:
        std::wstring                m_name;
        std::list<WeakPtr<Handler>> m_wpHandlers;
    };
}
