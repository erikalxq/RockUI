#ifndef __ROCK_RGN_H__
#define __ROCK_RGN_H__

namespace RockUI
{
    class RRgn
    {
    public:
        RRgn();
        virtual ~RRgn();
        operator HRGN() const { return m_hRgn; }
        bool IsValid();
        bool DeleteObject();
        HRGN CreateRectRgnIndirect(LPCRECT lpRect);
        HRGN CreatePolygonRgn(LPPOINT lpPoints, int nCount, int nMode);
        BOOL RectInRegion(LPCRECT lpRect);
        int CombineRgn(HRGN hRgnSrc, int nCombineMode);
        int GetRgnBox(LPRECT lpRect);

    private:
        HRGN m_hRgn;
    };
}

#endif