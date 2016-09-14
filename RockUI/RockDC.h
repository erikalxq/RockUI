#ifndef __ROCK_DC_H__
#define __ROCK_DC_H__

namespace RockUI
{
    class RDC
    {
    public:
        RDC();
        RDC(HDC dc);
        virtual ~RDC();
        operator HDC() const {return m_hDC;}
        bool IsValid();
        void Attach(HDC hDC);
        HDC Detach();
        bool DeleteDC();
        HDC CreateCompatibleDC(HDC hDC);
        HBITMAP SelectBitmap(HBITMAP hBitmap);
        HRGN SelectRgn(HRGN hRgn);

        //> 获取当前dc选择的bitmap句柄
        HBITMAP GetCurrentBitmap();
        //> 获取当前dc选择的bitmap信息
        bool GetCurrentBitmapInfo(BITMAP& bm);

    protected:
        void _Reset();

    protected:
        HDC m_hDC;
        bool m_bOwned;
    };

    class RWndDC : public RDC
    {
    public:
        RWndDC(HWND hWnd)
        {
            ATLASSERT(::IsWindow(hWnd));
            m_hWnd = hWnd;
            m_hDC = ::BeginPaint(hWnd, &m_ps);
        }
        ~RWndDC()
        {
            ATLASSERT(m_hDC != NULL);
            ATLASSERT(::IsWindow(m_hWnd));
            ::EndPaint(m_hWnd, &m_ps);
            m_hDC = NULL;
        }

    private:
        HWND m_hWnd;
        PAINTSTRUCT m_ps;
    };
}

#endif