#include "JackCTimer.h"

#ifdef _WINDOWS
#include <Windows.h>
#else
//TODO-请添加相应系统版本实现
#endif

#include <map>

#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"
using namespace JackC;

#ifdef _WINDOWS
static std::map<UINT_PTR, JackCTimer*> TimerMap;
void CALLBACK JackCTimer::TimerProc(HWND, UINT, UINT_PTR idEvent, DWORD dwTime)
{
    std::map<UINT_PTR, JackCTimer*>::iterator it = TimerMap.find(idEvent);
    if (it == TimerMap.end())
    {
        return;
    }

    JackCTimer *t = it->second;
    (*(t->m_callback))(t->m_userData, dwTime);
}
#else
//请添加相应系统版本实现
#endif

JackCTimer::JackCTimer(int interval, TimerCallback callback, void *userData)
    : m_interval(interval)
    , m_timerId(-1)
    , m_callback(callback)
    , m_userData(userData)
{

}
JackCTimer::~JackCTimer()
{
    Stop();
}


void JackCTimer::SetInterval(int msec)
{
#ifdef _WINDOWS
    m_interval = msec;
    if (IsActive())
    {
        ::SetTimer(0, m_timerId, m_interval, &JackCTimer::TimerProc);
    }
#endif
}
int JackCTimer::GetInterval() const
{
    return m_interval;
}

//! @brief 启动定时器，启动成功后回调函数按照指定的时间间隔定期触发
void JackCTimer::Start()
{
#ifdef _WINDOWS
    m_timerId = ::SetTimer(0, 0, m_interval, &JackCTimer::TimerProc);
    if (m_timerId != 0)
    {
        TimerMap.insert(std::make_pair(m_timerId, this));
    }
#endif
}

//! @brief 停止定时器
void JackCTimer::Stop()
{
#ifdef _WINDOWS
    if (m_timerId != -1)
    {
        if (::KillTimer(0, m_timerId))
        {
            std::map<UINT_PTR, JackCTimer*>::iterator it = TimerMap.find(m_timerId);
            if (it != TimerMap.end())
            {
                TimerMap.erase(it);
            }
            m_timerId = -1;
        }
    }
#endif
}