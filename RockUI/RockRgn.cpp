#include "stdafx.h"
#include "RockRgn.h"

namespace RockUI
{
    
    RRgn::RRgn() : m_hRgn(NULL)
    {

    }

    RRgn::~RRgn()
    {
        DeleteObject();
    }

    bool RRgn::IsValid()
    {
        return m_hRgn != NULL;
    }

    bool RRgn::DeleteObject()
    {
        if (!IsValid())
            return true;

        if (::DeleteObject(m_hRgn))
        {
            m_hRgn = NULL;
            return true;
        }
        return false;
    }

    HRGN RRgn::CreateRectRgnIndirect(LPCRECT lpRect)
    {
        ATLASSERT(!IsValid());
        m_hRgn = ::CreateRectRgnIndirect(lpRect);
        return m_hRgn;
    }

    HRGN RRgn::CreatePolygonRgn(LPPOINT lpPoints, int nCount, int nMode)
    {
        ATLASSERT(!IsValid());
        m_hRgn = ::CreatePolygonRgn(lpPoints, nCount, nMode);
        return m_hRgn;
    }

    BOOL RRgn::RectInRegion(LPCRECT lpRect)
    {
        ATLASSERT(IsValid());
        return ::RectInRegion(m_hRgn, lpRect);
    }

    int RRgn::CombineRgn(HRGN hRgnSrc, int nCombineMode)
    {
        ATLASSERT(IsValid());
        return ::CombineRgn(m_hRgn, m_hRgn, hRgnSrc, nCombineMode);
    }

    int RRgn::GetRgnBox(LPRECT lpRect)
    {
        ATLASSERT(IsValid());
        return ::GetRgnBox(m_hRgn, lpRect);
    }

}