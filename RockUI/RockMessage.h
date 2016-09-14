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

            //> bResult = 0，代表收到WM_QUIT消息
            //> bResult = -1，代表收到错误消息，可以使用GetLastError查看
            //> hWnd指定为NULL，则处理当前线程所有消息
            //> wMsgFilterMin，wMsgFilterMax设置为0，则接收所有有效的消息
            //> WM_KEYFIRST + WM_KEYLAST为键盘消息，WM_MOUSEFIRST + WM_MOUSELAST为鼠标消息
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
                    //>需要在无消息时处理的逻辑
                }
            }
        }
    };
}

#endif