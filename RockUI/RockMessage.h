#ifndef __ROCK_MESSAGE_H__
#define __ROCK_MESSAGE_H__

namespace RockUI
{
    class RMessage
    {
    public:
        static void CommonMessageLoop()
        {
            BOOL bResult = 0;
            MSG msg;

            //> bResult = 0�������յ�WM_QUIT��Ϣ
            //> bResult = -1�������յ�������Ϣ������ʹ��GetLastError�鿴
            //> hWndָ��ΪNULL������ǰ�߳�������Ϣ
            //> wMsgFilterMin��wMsgFilterMax����Ϊ0�������������Ч����Ϣ
            //> WM_KEYFIRST + WM_KEYLASTΪ������Ϣ��WM_MOUSEFIRST + WM_MOUSELASTΪ�����Ϣ
            while ( bResult = ::GetMessage(&msg, NULL, 0, 0) )
            {
                if (bResult == -1)
                    continue;

                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
        }

        static void SpecialMessageLoop()
        {
            MSG msg;
            while (TRUE)
            {
                if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
                {
                    if (msg.message == WM_QUIT)
                        break;
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
                else
                {
                    //>��Ҫ������Ϣʱ������߼�
                }
            }
        }
    };
}

#endif