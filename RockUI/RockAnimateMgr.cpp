#include "stdafx.h"
#include "RockAnimateWorker.h"
#include "RockWindow.h"

namespace RockUI
{
    RAnimateMgr* RAnimateMgr::s_animate = NULL;
    long RAnimateMgr::s_animateId = 1;

    RAnimateMgr::~RAnimateMgr()
    {
        CloseHandle(m_hTaskEvent);
    }

    void RAnimateMgr::AddAnimate(float fStart, float fEnd, float fInterval, float fTotalTime, bool bLoop, 
        HWND hwnd, eEaseTween eEaseType /*= ease_Linear*/, eInOutType eInOut /*= ease_in*/)
    {
        RAnimateTask* pTask = new RAnimateTask;
        pTask->fStart = fStart;
        pTask->fEnd = fEnd;
        pTask->fInterval = fInterval;
        pTask->fTotalTime = fTotalTime;
        pTask->bLoop = bLoop;
        pTask->m_hwnd = hwnd;
        pTask->eEaseType = eEaseType;
        pTask->eInOut = eInOut;
        EnterCriticalSection(&m_cs);
        m_animateTasks.push_back(pTask);
        LeaveCriticalSection(&m_cs);
        SetEvent(m_hTaskEvent);
    }

    RAnimateMgr* RAnimateMgr::GetInstance()
    {
        if (!s_animate)
            s_animate = new RAnimateMgr;
        return s_animate;
    }

    void RAnimateMgr::Release()
    {
        if (s_animate)
        {
            delete s_animate;
            s_animate = NULL;
        }
    }

    RAnimateMgr::RAnimateMgr()
    {
        InitializeCriticalSection(&m_cs);
        m_hTaskEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    }

}