#include "DateTime.h"
#include <ctime>
#include "DebugMessage.h"
#include "StringUtils.h"

#ifdef _WINDOWS
#include "Windows.h"
#else
#include <sys/time.h>
#include <cstring>
#endif

#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"

using namespace JackC;


DateTime::DateTime():m_time(0)
{

}

DateTime::DateTime(uint64_t time):m_time(time)
{

}

DateTime::DateTime(int year,int month,int day)
{
    DBG_WARN_UNLESS(year > 1900 && month >= 0 && month < 12 && day >= 1 && day <= 31,
        L"无效参数",
        L"jack.chen",
        L"2025-07-19");

    tm t;
    memset(&t, 0, sizeof(t));
    t.tm_year = year - 1900;
    t.tm_mon = month - 1;
    t.tm_mday = day;
    m_time = mktime(&t);
}

DateTime::~DateTime()
{

}

DateTime DateTime::Now()
{
    DateTime dt;
    time((time_t*)&dt.m_time); //取得从1970年1月1日至今的秒数。
    return dt;
}

int DateTime::GetDay() const
{
    tm *pTime = localtime((time_t*)&m_time);
    DBG_WARN_AND_RETURN_UNLESS(pTime,0, L"无效参数", L"jack.chen", L"2025-07-19");
    return pTime->tm_mday;
}

int DateTime::GetMonth() const
{
    tm *pTime = localtime((time_t*)&m_time);
    DBG_WARN_AND_RETURN_UNLESS(pTime, 0, L"无效参数", L"jack.chen", L"2025-07-19");
    return pTime->tm_mon + 1;
}

int DateTime::GetYear() const
{
    tm *pTime = localtime((time_t*)&m_time);
    DBG_WARN_AND_RETURN_UNLESS(pTime, 0, L"无效参数", L"jack.chen", L"2025-07-19");
    return pTime->tm_year + 1900;
}

int DateTime::GetHour() const
{
    tm *pTime = localtime((time_t*)&m_time);
    DBG_WARN_AND_RETURN_UNLESS(pTime, 0, L"无效参数", L"jack.chen", L"2025-07-19");
    return pTime->tm_hour;
}

int DateTime::GetMinute() const
{
    tm *pTime = localtime((time_t*)&m_time);
    DBG_WARN_AND_RETURN_UNLESS(pTime, 0, L"无效参数", L"jack.chen", L"2025-07-19");
    return pTime->tm_min;
}

int DateTime::GetSeconds() const
{
    tm *pTime = localtime((time_t*)&m_time);
    DBG_WARN_AND_RETURN_UNLESS(pTime, 0, L"无效参数", L"jack.chen", L"2025-07-19");
    return pTime->tm_sec;
}

uint64_t DateTime::GetTime() const
{
    return m_time;
}

std::wstring DateTime::ToString() const
{
    if (tm *pTime = localtime((time_t*)&m_time))
    {
        const int size = 64;
        char tmp[size];
        strftime(tmp, sizeof(tmp), "%Y-%m-%d %X", pTime);
        return StringUtils::ToWString(tmp);
    }
    else
    {
        return L"";
    }
}

std::wstring DateTime::ToDateString() const
{
    if (tm *pTime = localtime((time_t*)&m_time))
    {
        const int size = 64;
        char tmp[size];
        strftime(tmp, sizeof(tmp), "%Y-%m-%d", pTime);
        return StringUtils::ToWString(tmp);
    }
    else
    {
        return L"";
    }
}

uint64_t JackC::DateTime::GetSystemTimeMicros()
{
#ifdef _WINDOWS
    // 从1601年1月1日0:0:0:000到1970年1月1日0:0:0:000的时间(单位100ns)
#define EPOCHFILETIME   (116444736000000000UL)
    FILETIME ft;
    LARGE_INTEGER li;
    uint64_t tt = 0;
    GetSystemTimeAsFileTime(&ft);
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;
    // 从1970年1月1日0:0:0:000到现在的微秒数(UTC时间)
    tt = (li.QuadPart - EPOCHFILETIME) / 10;
    return tt;
#else
    timeval tv;
    gettimeofday(&tv, 0);
    return (uint64_t)tv.tv_sec * 1000000 + (uint64_t)tv.tv_usec;
#endif // _WINDOWS
}
