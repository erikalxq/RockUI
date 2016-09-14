#ifndef __ROCK_WINDOW_H__
#define __ROCK_WINDOW_H__

namespace RockUI
{
    typedef CWinTraits<WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN | WS_TABSTOP, WS_EX_CONTROLPARENT> RWinTraits;

    template <class T, class TBase = ATL::CWindow, class TWinTraits = RWinTraits>
    class RWindow 
        : public ATL::CWindowImpl<T, TBase, TWinTraits>
    {
    public:
        //> CS_DBLCLKS：windows向窗口发送双击事件
        //> CS_HREDRAW | CS_VREDRAW：当窗口水平、垂直方向的宽度变化时重绘整个窗口
        //> COLOR_WINDOW：默认背景色
        DECLARE_WND_CLASS_EX(NULL, CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, COLOR_WINDOW)

        RWindow() : m_pLayer(NULL)
        {
        }
        ~RWindow()
        {
            if (m_pLayer)
                m_pLayer->Destroy();
            m_bmpMem.DeleteObject();
            m_invalidateRgn.DeleteObject();
        }
        RLayer* GetCanvas()
        {
            return m_pLayer;
        }
        HWND Create(HWND hWndParent, _U_RECT rect = NULL, LPCTSTR szWindowName = NULL, DWORD dwStyle = 0, DWORD dwExStyle = 0, _U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL)
        {
            HWND hWnd = __super::Create(hWndParent, rect, szWindowName, dwStyle, dwExStyle, MenuOrID, lpCreateParam);
            if (hWnd)
            {
                m_pLayer = new RLayer(NULL, hWnd);
                //> 默认开启layered属性
                ModifyStyleEx(0, WS_EX_LAYERED);
            }
            return hWnd;
        }

    protected:
        void OnPaint(HDC dc)
        {
            CRect rcClient;
            GetClientRect(&rcClient);
            if (rcClient.IsRectNull() || rcClient.IsRectEmpty())
                return;

            if (!m_invalidateRgn.IsValid())
            {
                m_invalidateRgn.CreateRectRgnIndirect(&rcClient);
            }

            RWndDC wndDC(m_hWnd);

            m_bmpMem.SmartCreateCompatibleBitmap(wndDC, rcClient.Width(), rcClient.Height());
            //m_bmpMem.CleanRgnPix(m_invalidateRgn);

            RDC dcMem;
            HDC hdcMem = dcMem.CreateCompatibleDC(wndDC);
            if (hdcMem == NULL)
                return;

            HBITMAP hOldBmp = dcMem.SelectBitmap(m_bmpMem);
            HRGN hOldRgn = dcMem.SelectRgn(m_invalidateRgn);

            if (m_pLayer)
                m_pLayer->Redraw(dcMem, m_invalidateRgn);

            CRect rcWnd;
            GetWindowRect(&rcWnd);
            POINT ptSrc, ptDes;
            SIZE szDes;
            ptSrc.x = 0;
            ptSrc.y = 0;
            ptDes.x = rcWnd.left;
            ptDes.y = rcWnd.top;
            szDes.cx = rcWnd.Width();
            szDes.cy = rcWnd.Height();

            BLENDFUNCTION blend;
            blend.BlendOp = AC_SRC_OVER;
            blend.BlendFlags = 0;
            blend.AlphaFormat = AC_SRC_ALPHA;
            blend.SourceConstantAlpha = 255;

            ::UpdateLayeredWindow(m_hWnd, wndDC, &ptDes, &szDes, dcMem, &ptSrc, RGB(0, 0, 0), &blend, ULW_ALPHA);

            HBITMAP HB = dcMem.SelectBitmap(hOldBmp);
            HRGN h = dcMem.SelectRgn(hOldRgn);

            m_invalidateRgn.DeleteObject();
        }

        void OnSize(UINT nType, CSize size)
        {
            if (m_pLayer)
                m_pLayer->Resize();
        }

        LRESULT OnNotifyInvalidate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bHandled)
        {
            CRect* pInvalidateRect = (CRect*)wParam;
            HRITEM hItem = (HRITEM)lParam;
            if (!pInvalidateRect || pInvalidateRect->IsRectEmpty() || pInvalidateRect->IsRectNull())
                return S_FALSE;

            if (m_invalidateRgn.IsValid())
            {
                RRgn newRgn;
                newRgn.CreateRectRgnIndirect(pInvalidateRect);
                m_invalidateRgn.CombineRgn(newRgn, RGN_OR);
            }
            else
            {
                m_invalidateRgn.CreateRectRgnIndirect(pInvalidateRect);
            }
            InvalidateRect(pInvalidateRect, FALSE);

            return S_OK;
        }

        void OnLButtonDown(UINT nFlags, CPoint point)
        {
            SendMessage(WM_SYSCOMMAND, SC_MOVE | HTCAPTION);
        }

        BEGIN_MSG_MAP_EX(RWindow)
            MSG_WM_PAINT(OnPaint)
            MSG_WM_SIZE(OnSize)
            MSG_WM_LBUTTONDOWN(OnLButtonDown)
            MESSAGE_HANDLER(ROCKWM_INVALIDATE, OnNotifyInvalidate)
        END_MSG_MAP()

    private:
        RLayer* m_pLayer;
        RBitmap m_bmpMem;
        RRgn m_invalidateRgn;
    };
}
#endif