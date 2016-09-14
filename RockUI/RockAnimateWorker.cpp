#include "StdAfx.h"
#include "utils/RockClock.h"
#include "RockAnimateWorker.h"

namespace RockUI
{
    RAnimateWorker::RAnimateWorker(HANDLE hTaskEvent, CRITICAL_SECTION cs, RAnimateTasks* tasks)
    : m_hTaskEvent(hTaskEvent),
      m_cs(cs),
      m_TaskList(tasks)
    {
        m_clockThread.SetExecutor(this);
        m_clockThread.Execute(&RAnimateWorker::_ClockThread);
    }

    RAnimateWorker::~RAnimateWorker(void)
    {
        if (m_clockThread.IsTaskExecuting())
            m_clockThread.Wait(WAIT_OBJECT_0, INFINITE);
    }

    int RAnimateWorker::_ClockThread(LPVOID pParam)
    {
        RClock clock;
        ULONGLONG curMs;
        while(true)
        {
            Sleep(2);

            DWORD dwRet = WaitForSingleObject(m_hTaskEvent, m_myTasks.empty() ? INFINITE : 0);
            if (dwRet == WAIT_OBJECT_0)
                _GetNewTask();

            EnterCriticalSection(&m_cs);
            for (RAnimateTasks::iterator it = m_myTasks.begin(); it != m_myTasks.end();)
            {
                clock.NowMs(curMs);
                RAnimateTask* pTask = *it;
                if ( (curMs - pTask->lastTime) >= pTask->fInterval )
                {
                    pTask->datas->AddTime(pTask->fInterval);
                    pTask->fCur = pTask->datas->Caculate();
                    if (pTask->m_hwnd)
                        PostMessage(pTask->m_hwnd, ROCK_ANIMATE_UPDATE_EVENT, pTask->id, NULL);
                    pTask->lastTime = curMs;
                }
                if (pTask->datas->IsStop() && !pTask->bLoop)
                {
                    pTask->datas->Clear();
                    it = m_myTasks.erase(it);
                }
                else if (pTask->datas->IsStop())
                {
                    pTask->datas->Clear();
                    pTask->datas->SetValue(pTask->fTotalTime, pTask->fStart, pTask->fEnd);
                    it ++;
                }
                else
                {
                    it ++;
                }
            }
            LeaveCriticalSection(&m_cs);
        }
    }

    BOOL RAnimateWorker::_GetNewTask()
    {
        BOOL bResult = FALSE;
        RAnimateTask* pTask = NULL;
        do 
        {
            ::EnterCriticalSection(&m_cs);

            if (m_TaskList == NULL)
                break;

            if (m_TaskList->empty())
                break;

            RAnimateTask* pTask = m_TaskList->front();
            m_TaskList->pop_front();

            if (!pTask)
                break;

            m_myTasks.push_back(pTask);
        } while (0);

        ::LeaveCriticalSection(&m_cs);

       _InitTaskAnimateEaseType(pTask);
       pTask->datas->SetValue(pTask->fTotalTime, pTask->fStart, pTask->fEnd);
       RClock c;
       c.NowMs(pTask->lastTime);

        return bResult;
    }

    BOOL RAnimateWorker::_InitTaskAnimateEaseType(RAnimateTask* pTask)
    {
        switch(pTask->eEaseType)
        {
        case ease_Linear:
            pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::Linear<float> >;
            break;
        case ease_Quadratic:
            if (pTask->eInOut == ease_in)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::IQuad<float> >;
            else if (pTask->eInOut == ease_out)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::OQuad<float> >;
            else if (pTask->eInOut == ease_inOut)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::IOQuad<float> >;
            else if (pTask->eInOut == ease_outIn)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::OIQuad<float> >;
            break;
        case ease_Cubic:
            if (pTask->eInOut == ease_in)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::ICubic<float> >;
            else if (pTask->eInOut == ease_out)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::OCubic<float> >;
            else if (pTask->eInOut == ease_inOut)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::IOCubic<float> >;
            else if (pTask->eInOut == ease_outIn)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::OICubic<float> >;
            break;
        case ease_Quartic:
            if (pTask->eInOut == ease_in)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::IQuart<float> >;
            else if (pTask->eInOut == ease_out)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::OQuart<float> >;
            else if (pTask->eInOut == ease_inOut)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::IOQuart<float> >;
            else if (pTask->eInOut == ease_outIn)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::OIQuart<float> >;
            break;
        case ease_Quintic:
            if (pTask->eInOut == ease_in)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::IQuint<float> >;
            else if (pTask->eInOut == ease_out)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::OQuint<float> >;
            else if (pTask->eInOut == ease_inOut)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::IOQuint<float> >;
            else if (pTask->eInOut == ease_outIn)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::OIQuint<float> >;
            break;
        case ease_Sinusoidal:
            if (pTask->eInOut == ease_in)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::ISine<float> >;
            else if (pTask->eInOut == ease_out)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::OSine<float> >;
            else if (pTask->eInOut == ease_inOut)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::IOSine<float> >;
            else if (pTask->eInOut == ease_outIn)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::OISine<float> >;
            break;
        case ease_Exponential:
            if (pTask->eInOut == ease_in)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::IExpo<float> >;
            else if (pTask->eInOut == ease_out)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::OExpo<float> >;
            else if (pTask->eInOut == ease_inOut)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::IOExpo<float> >;
            else if (pTask->eInOut == ease_outIn)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::OIExpo<float> >;
            break;
        case ease_Circular:
            if (pTask->eInOut == ease_in)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::ICirc<float> >;
            else if (pTask->eInOut == ease_out)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::OCirc<float> >;
            else if (pTask->eInOut == ease_inOut)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::IOCirc<float> >;
            else if (pTask->eInOut == ease_outIn)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::OICirc<float> >;
            break;
        case ease_Elastic:
            if (pTask->eInOut == ease_in)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::IElastic<float> >;
            else if (pTask->eInOut == ease_out)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::OElastic<float> >;
            else if (pTask->eInOut == ease_inOut)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::IOElastic<float> >;
            else if (pTask->eInOut == ease_outIn)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::OIElastic<float> >;
            break;
        case ease_Back:
            if (pTask->eInOut == ease_in)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::IBack<float> >;
            else if (pTask->eInOut == ease_out)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::OBack<float> >;
            else if (pTask->eInOut == ease_inOut)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::IOBack<float> >;
            else if (pTask->eInOut == ease_outIn)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::OIBack<float> >;
            break;
        case ease_Bounce:
            if (pTask->eInOut == ease_in)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::IBounce<float> >;
            else if (pTask->eInOut == ease_out)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::OBounce<float> >;
            else if (pTask->eInOut == ease_inOut)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::IOBounce<float> >;
            else if (pTask->eInOut == ease_outIn)
                pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::OIBounce<float> >;
            break;
        case ease_Parabola:
            pTask->datas = new ease_tween::KEaseTweenSimpleAnimateData< ease_tween::Parabola<float> >;
            break;
        }
        return TRUE;
    }

}