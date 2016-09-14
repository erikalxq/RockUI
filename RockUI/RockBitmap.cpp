#include "stdafx.h"
#include "RockBitmap.h"

namespace RockUI
{
    RBitmap::RBitmap() : m_hBitmap(NULL), m_bOwned(true), m_bytes(NULL)
    {

    }

    RBitmap::RBitmap(HBITMAP hBitmap) : m_bOwned(false), m_bytes(NULL)
    {
        Attach(hBitmap);
    }

    RBitmap::~RBitmap()
    {
        DeleteObject();
    }

    bool RBitmap::DeleteObject()
    {
        if (!IsValid())
            return true;

        if (!m_bOwned)
        {
            Detach();
            return true;
        }

        if (::DeleteObject(m_hBitmap))
        {
            m_hBitmap = NULL;
            return true;
        }
        return false;
    }

    bool RBitmap::GetBitmap(BITMAP& pBitMap)
    {
        ATLASSERT(IsValid());
        return (::GetObject(m_hBitmap, sizeof(BITMAP), &pBitMap) == sizeof(BITMAP));
    }

    bool RBitmap::CleanAllPix()
    {
        ATLASSERT(IsValid());
        BITMAP bm;
        ATLASSERT(GetBitmap(bm));
        ZeroMemory(bm.bmBits, bm.bmWidth * bm.bmHeight * bm.bmBitsPixel / 8);
        return true;
    }

    bool RBitmap::CleanRgnPix(RRgn& rgn)
    {
        ATLASSERT(IsValid());

        if (!rgn.IsValid())
            return false;

        RRgn rgnMem;
        BITMAP bm;
        ATLASSERT(GetBitmap(bm));
        CRect rgnRect(0, 0, bm.bmWidth, bm.bmHeight);
        rgnMem.CreateRectRgnIndirect(&rgnRect);
        rgnMem.CombineRgn(rgn, RGN_DIFF);

        RDC rdcMem;
        RBitmap rbitmapMem;
        rdcMem.CreateCompatibleDC(NULL);
        rbitmapMem.CreateDIBSection(bm.bmWidth, bm.bmHeight);

        HBITMAP hOldBitmap = rdcMem.SelectBitmap(rbitmapMem);
        rdcMem.SelectRgn(rgnMem);

        RDC rdcSrc;
        rdcSrc.CreateCompatibleDC(rdcMem);
        HBITMAP hOldBitmap2 = rdcSrc.SelectBitmap(m_hBitmap);

        BLENDFUNCTION Blend;
        Blend.BlendOp = 0;
        Blend.BlendFlags = 0;
        Blend.AlphaFormat = AC_SRC_ALPHA;
        Blend.SourceConstantAlpha = (BYTE)255;

        ::AlphaBlend(rdcMem, 0, 0, bm.bmWidth, bm.bmHeight, rdcSrc, 0, 0, bm.bmWidth, bm.bmHeight, Blend);
        rdcMem.SelectBitmap(hOldBitmap);
        rdcSrc.SelectBitmap(hOldBitmap2);

        Attach(rbitmapMem, true);

        return true;
    }

    HBITMAP RBitmap::CreateCompatibleBitmap(HDC hDC, int iWidth, int iHeight)
    {
        ATLASSERT(!IsValid());
        m_hBitmap = ::CreateCompatibleBitmap(hDC, iWidth, iHeight);
        return m_hBitmap;
    }

    HBITMAP RBitmap::SmartCreateCompatibleBitmap(HDC hDC, int iWidth, int iHeight)
    {
        if (IsValid())
        {
            BITMAP bm;
            ATLASSERT(GetBitmap(bm));
            if (bm.bmWidth != iWidth || bm.bmHeight != iHeight)
            {
                ATLASSERT(DeleteObject());
            }
            else
            {
                return m_hBitmap;
            }
        }
        m_hBitmap = ::CreateCompatibleBitmap(hDC, iWidth, iHeight);
        return m_hBitmap;
    }

    HBITMAP RBitmap::CreateDIBSection(int biWidth, int biHeight, int biPlanes /*= 1*/, int biBitCount /*= 32*/)
    {
        if (biWidth < 0 || biHeight < 0)
            return NULL;

        BITMAPINFOHEADER bmih;
        ZeroMemory(&bmih, sizeof(BITMAPINFOHEADER));

        bmih.biSize = sizeof(BITMAPINFOHEADER);
        bmih.biWidth = biWidth;
        bmih.biHeight = biHeight;
        bmih.biPlanes = biPlanes;
        bmih.biBitCount = biBitCount;
        bmih.biCompression = BI_RGB;

        DeleteObject();

        HDC hDestopDC = ::GetDC(NULL);
        m_hBitmap = ::CreateDIBSection(hDestopDC, (BITMAPINFO*)&bmih, 0, (VOID**)&m_bytes, NULL, 0);
        ::ReleaseDC(NULL, hDestopDC);

        return m_hBitmap;
    }

    bool RBitmap::IsValid()
    {
        return m_hBitmap != NULL;
    }

    void RBitmap::Attach(RBitmap& bitmap, bool bOwned /*= false*/)
    {
        DeleteObject();
        m_hBitmap = bitmap.Detach();
        m_bOwned = bOwned;
    }

    void RBitmap::Attach(HBITMAP hBitmap, bool bOwned /*= false*/)
    {
        DeleteObject();
        m_hBitmap = hBitmap;
        m_bOwned = bOwned;
    }

    HBITMAP RBitmap::Detach()
    {
        HBITMAP hBitmap = m_hBitmap;
        m_hBitmap = NULL;
        return hBitmap;
    }
}