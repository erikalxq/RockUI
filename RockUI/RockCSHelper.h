#ifndef __ROCK_CSHELPER_H__
#define __ROCK_CSHELPER_H__

namespace RockUI
{
    class RCSHelper
    {
    public:
        RCSHelper()
        {
            ::InitializeCriticalSection(&m_locker);
            ::EnterCriticalSection(&m_locker);
        }
        ~RCSHelper()
        {
            ::LeaveCriticalSection(&m_locker);
            ::DeleteCriticalSection(&m_locker);
        }

    private:
        CRITICAL_SECTION m_locker;
    };
}

#endif