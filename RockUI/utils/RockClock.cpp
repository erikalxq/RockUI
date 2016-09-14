#include "stdafx.h"
#include "RockClock.h"

RClock::RClock()
{
    GetSystemTime(&m_startTime);
}

RClock::~RClock()
{

}

void RClock::Cal(char* msg)
{
    GetSystemTime(&m_endTime);
    int elipseTime = (m_endTime.wMinute - m_startTime.wMinute) * 60 * 1000 + (m_endTime.wSecond - m_startTime.wSecond) * 1000 + (m_endTime.wMilliseconds - m_startTime.wMilliseconds);
    printf("%s : %d ms; \r\n", msg, elipseTime);
}

void RClock::NowMs(ULONGLONG& now)
{
    SYSTEMTIME t;
    FILETIME ft;
    GetSystemTime(&t);
    FileTimeToSystemTime(&ft, &t);
    ULARGE_INTEGER ut;
    ut.HighPart = ft.dwHighDateTime;
    ut.LowPart = ft.dwLowDateTime;
    now = ut.QuadPart / 1000 / 1000;
}
