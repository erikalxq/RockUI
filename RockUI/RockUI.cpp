// RockUI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "RockWindow.h"
#include "RockMessage.h"
#include "RockImage.h"
#include "shlobj.h"

using namespace RockUI;

class testWnd : public RWindow<testWnd>
{
public:
    testWnd(){}
    ~testWnd(){}
};

int _tmain(int argc, _TCHAR* argv[])
{
    RockUIEnv env;
    RImagePool::GetInstance()->AddResourcePath(_T("E:"));
    testWnd* pWnd = new testWnd;
    HWND hWnd = pWnd->Create(NULL);
    RLayer* pCanvas = pWnd->GetCanvas();
    RImage* pImage = new RImage;
    pImage->SetResource(_T("桌面整理.png"));
    pCanvas->AttachChild(pImage->GetHItem());
    /*for (int i = 0; i < 20; i++)
    {
        RImage* pImage2 = new RImage;
        pImage2->SetResource(_T("桌面整理.png"));
        pImage2->SetLeft(10 * i);
        pImage2->SetTop(10 * i);
        pImage2->SetWidth(300);
        pImage2->SetHeight(300);
        pImage->AttachChild(pImage2->GetHItem());
    }*/
    
    /*RImage* pImage2 = new RImage;
    pImage2->SetResource(_T("桌面整理.png"));
    pImage2->SetLeft(10);
    pImage2->SetTop(10);
    pImage2->SetWidth(300);
    pImage2->SetHeight(300);
    pImage->AttachChild(pImage2->GetHItem());*/

    pWnd->MoveWindow(CRect(0,0,600,400));
    RMessage::CommonMessageLoop();
    return 0;
}

