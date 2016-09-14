#ifndef __ROCK_ANIMATE_H__
#define __ROCK_ANIMATE_H__
#include "RockAnimateWorker.h"

namespace RockUI
{
    class RAnimateMgr
    {
    public:
        ~RAnimateMgr();
        void AddAnimate(float fStart, float fEnd, float fInterval, float fTotalTime, bool bLoop, HWND hwnd, eEaseTween eEaseType = ease_Linear, eInOutType eInOut = ease_in);
        static RAnimateMgr* GetInstance();
        static void Release();

    protected:
        RAnimateMgr();
        RAnimateMgr(const RAnimateMgr& t);
        RAnimateMgr& operator = (const RAnimateMgr& t);

    private:
        static RAnimateMgr* s_animate;
        static long s_animateId;
        RAnimateTasks m_animateTasks;
        HANDLE m_hTaskEvent;
        CRITICAL_SECTION m_cs;
    };
}

#endif