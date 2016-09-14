#include "stdafx.h"
#include "RockDC.h"

namespace RockUI
{

    RDC::RDC() : m_hDC(NULL)
    {
        _Reset();
    }

    RDC::RDC(HDC dc) : m_hDC(NULL)
    {
        _Reset();
        Attach(dc);
    }

    RDC::~RDC()
    {
        DeleteDC();
    }

    bool RDC::IsValid()
    {
        return m_hDC != NULL;
    }

    void RDC::Attach(HDC hDC)
    {
        ATLASSERT(DeleteDC());
        m_hDC = hDC;
        m_bOwned = false;
    }

    HDC RDC::Detach()
    {
        HDC hdc = m_hDC;
        m_hDC = NULL;
        return hdc;
    }

    HDC RDC::CreateCompatibleDC(HDC hDC)
    {
        ATLASSERT(!IsValid());
        m_hDC = ::CreateCompatibleDC(hDC);
        return m_hDC;
    }

    bool RDC::DeleteDC()
    {
        if (!IsValid())
        {
            return true;
        }
        else if (!m_bOwned)
        {
            Detach();
            return true;
        }
        else if (::DeleteDC(m_hDC))
        {
            m_hDC = NULL;
            return true;
        }
        return false;
    }

    HBITMAP RDC::SelectBitmap(HBITMAP hBitmap)
    {
        if (!IsValid())
            return NULL;

        if (::GetObjectType(hBitmap) == OBJ_BITMAP)
            return (HBITMAP)::SelectObject(m_hDC, hBitmap);

        return NULL;
    }

    HRGN RDC::SelectRgn(HRGN hRgn)
    {
        if (!IsValid())
            return NULL;

        if (::GetObjectType(hRgn) == OBJ_REGION)
            return (HRGN)::SelectObject(m_hDC, hRgn);

        return NULL;
    }

    HBITMAP RDC::GetCurrentBitmap()
    {
        ATLASSERT(IsValid());
        return (HBITMAP)::GetCurrentObject(m_hDC, OBJ_BITMAP);
    }

    bool RDC::GetCurrentBitmapInfo(BITMAP& bm)
    {
        RBitmap rBitmap(GetCurrentBitmap());
        if (!rBitmap.IsValid())
            return false;

        return rBitmap.GetBitmap(bm);
    }

    void RDC::_Reset()
    {
        ATLASSERT(DeleteDC());
        m_bOwned = true;
        m_hDC = NULL;
    }

}