#ifndef __ROCK_TIMER_H__
#define __ROCK_TIMER_H__

namespace RockUI
{
    class RTimer
    {
        class RTimerNotify
        {
        public:
            virtual ~RTimerNotify(){}
            virtual void RTimerNotify(long id);
        };

        struct TimerTask
        {
            long id;
            DWORD interval;
            DWORD lastTime;
            RTimerNotify* pListener;
            TimerTask() : pListener(NULL){}
        };
        typedef std::map<int, TimerTask> TimerTasks;

    public:
        ~RTimer();
        int AddTimer(DWORD interval, RTimerNotify* pNotify);
        void KillTimer(int id);
        static RTimer* Get();
        static void Release();

    protected:
        RTimer();
        RTimer(const RTimer& t);
        RTimer& operator = (const RTimer& t);
        void _Init();
        void _Unit();
        int _ThreadTimer( LPVOID pParam );

    private:
        TimerTasks m_timerTasks;
        KThreadTask<RTimer> m_threadTimer;
        HANDLE m_hExitEvent;
        void* m_pNotifyWnd;
        static RTimer* s_timer;
        static long s_timerId;
    };
}

#endif