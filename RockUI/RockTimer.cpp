#include "stdafx.h"
#include "RockTimer.h"

namespace RockUI
{
    RTimer* RTimer::s_timer = NULL;
    long RTimer::s_timerId = 1;

    class RNotifyWnd : public RWindow<RNotifyWnd>
    {
    public:
        RNotifyWnd(){}
        ~RNotifyWnd(){}
        LRESULT OnNotifyTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bHandled)
        {
            return S_OK;
        }

    protected:
        BEGIN_MSG_MAP_EX(RNotifyWnd)
            MESSAGE_HANDLER(ROCKWM_TIMER, OnNotifyTimer)
            END_MSG_MAP()
    };

    RTimer::RTimer()
    {
        _Init();
    }

    void RTimer::_Init()
    {
        m_hExitEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
        m_threadTimer.SetExecutor(this);
        m_threadTimer.Execute(&RTimer::_ThreadTimer);
        RNotifyWnd* pNotifyWnd = new RNotifyWnd;
        pNotifyWnd->Create(NULL);
        m_pNotifyWnd = pNotifyWnd;
    }

    void RTimer::_Unit()
    {
        ::SetEvent(m_hExitEvent);
        if (m_threadTimer.IsTaskExecuting())
        {
            m_threadTimer.Stop();
        }
        ::CloseHandle(m_hExitEvent);

        if (m_pNotifyWnd)
        {
            RNotifyWnd* pNotifyWnd = (RNotifyWnd*)m_pNotifyWnd;
            pNotifyWnd->DestroyWindow();
            delete pNotifyWnd;
            m_pNotifyWnd = NULL;
        }
    }

    int ThreadTimer::_ThreadTimer(LPVOID pParam)
    {
        while (true)
        {
            if (WaitForSingleObject(m_hExitEvent, 1) == WAIT_OBJECT_0)
                return 1;

            for (TimerTasks::iterator it = m_timerTasks.begin(); it != m_timerTasks.end(); ++it)
            {
                TimerTask& task = it->second;
                DWORD dwCurrentTime = GetTickCount();
                if (dwCurrentTime - task.lastTime > task.interval)
                {
                    task.interval = dwCurrentTime;
                    if (task.pListener)
                        task.pListener->ThreadTimerNotify(task.id);
                }
            }
        }
        return 0;
    }

    ThreadTimer::~ThreadTimer()
    {
        _Unit();
    }

    int ThreadTimer::AddTimer(DWORD interval, TimerNotify* pNotify)
    {
        TimerTask& task = m_timerTasks[s_timerId++];
        task.interval = interval;
        task.lastTime = GetTickCount();
        task.pListener = pNotify;
        return task.id;
    }

    void ThreadTimer::KillTimer(int id)
    {
        m_timerTasks.erase(id);
    }

    ThreadTimer* ThreadTimer::Get()
    {
        if (!s_timer)
            s_timer = new ThreadTimer;

        return s_timer;
    }

    void ThreadTimer::Release()
    {
        if (s_timer)
        {
            delete s_timer;
            s_timer = NULL;
        }
    }
}