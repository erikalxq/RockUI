#pragma once
#include "utils/kthreadtask.h"
#include "utils/animate_data_def.h"
#include "utils/CEaseTween.h"

#define ROCK_ANIMATE_UPDATE_EVENT   1048

namespace RockUI
{
    enum eEaseTween
    {
        ease_Linear = 0,
        ease_Quadratic,
        ease_Cubic,
        ease_Quartic,
        ease_Quintic,
        ease_Sinusoidal,
        ease_Exponential,
        ease_Circular,
        ease_Elastic,
        ease_Back,
        ease_Bounce,
        ease_Parabola
    };
    enum eInOutType
    {
        ease_in = 0,
        ease_out,
        ease_inOut,
        ease_outIn
    };
    struct RAnimateTask
    {
        long id;
        float fInterval;
        float fStart;
        float fEnd;
        float fCur;
        float fTotalTime;
        bool bLoop;
        HWND m_hwnd;
        ULONGLONG lastTime;
        eEaseTween eEaseType;
        eInOutType eInOut;
        ease_tween::IKEaseTweenSimpleAnimateData* datas;

        RAnimateTask(){memset(this, 0, sizeof(RAnimateTask));}
    };

    typedef std::list<RAnimateTask*> RAnimateTasks;
    class RAnimateWorker
    {
    public:
        RAnimateWorker(HANDLE hTaskEvent, CRITICAL_SECTION cs, RAnimateTasks* tasks);
        ~RAnimateWorker(void);
        typedef KThreadTask<RAnimateWorker> KClockThread;

    protected:
        int _ClockThread(LPVOID pParam);
        BOOL _GetNewTask();
        BOOL _InitTaskAnimateEaseType(RAnimateTask* pTask);

    private:
        KClockThread m_clockThread;
        HANDLE m_hTaskEvent;
        RAnimateTasks m_myTasks;
        RAnimateTasks* m_TaskList;
        CRITICAL_SECTION m_cs;
    };
}