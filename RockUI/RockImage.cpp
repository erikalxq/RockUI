#include "stdafx.h"
#include "RockImage.h"
#include "../skia/include/core/SkBitmap.h"
#include "../skia/include/core/SKCanvas.h"
#include "../skia/include/effects/SkGradientShader.h"
#include "../skia/include/effects/SKBlurMaskFilter.h"

namespace RockUI
{

    RImage::RImage(HRITEM hParentItem /*= NULL*/, HWND hContainer /*= NULL*/) : RLayer(hParentItem, hContainer)
    {

    }

    RImage::~RImage()
    {

    }

    void RImage::SetResource(std::wstring sResourceFile)
    {
        m_sResourceFile = sResourceFile;
    }

    void RImage::Redraw(HDC dc, RRgn& invalidateRgn)
    {
        RClock clock;

        CRect rc;
        GetRect(rc);

        //_UpdateMyBitmap(dc);

        clock.Cal("update my bitmap");

        RDC dcMem;
        dcMem.CreateCompatibleDC(dc);

        {
            RBitmap bm;
            bm.CreateDIBSection(rc.Width(), rc.Height());
            HBITMAP hOldBitmap = dcMem.SelectBitmap(bm);
            SkBitmap sk_bitmap;
            SkImageInfo skImageInfo = SkImageInfo::MakeN32Premul(rc.Width(), rc.Height());
            sk_bitmap.allocPixels(skImageInfo);
            SkCanvas sk_canvas(sk_bitmap);

            SkPoint points[2] = {
                SkPoint::Make(0.0f, 0.0f),
                SkPoint::Make(256.0f, 256.0f)
            };
            SkColor colors[2] = {SK_ColorBLUE, SK_ColorYELLOW};
            SkPaint paint;
            paint.setShader(SkGradientShader::CreateLinear(
                points, colors, NULL, 2,
                SkShader::kClamp_TileMode, 0, NULL));
            sk_canvas.clear(SK_ColorWHITE);
            sk_canvas.drawPaint(paint);

            const SkScalar scale = 256.0f;
            const SkScalar R = 0.45f * scale;
            const SkScalar TAU = 6.2831853f;
            SkPath path;
            path.moveTo(R, 0.0f);
            for (int i = 1; i < 7; ++i) {
                SkScalar theta = 3 * i * TAU / 7;
                path.lineTo(R * cos(theta), R * sin(theta));
            }
            path.close();
            SkPaint p;
            p.setAntiAlias(true);
            sk_canvas.translate(0.5f * scale, 0.5f * scale);
            sk_canvas.drawPath(path, p);

            SkPaint paint2;
            paint2.setAntiAlias(true);
            paint2.setTextSize(120);
            paint2.setMaskFilter(SkBlurMaskFilter::Create(
                kNormal_SkBlurStyle, 5.0f, 0));
            const char text[] = "Skia";
            sk_canvas.drawText(text, strlen(text), 0, 160, paint2);

            /*SkPaint paint1, paint2, paint3;

            paint1.setTextSize(64.0f);
            paint1.setAntiAlias(true);
            paint1.setColor(SkColorSetRGB(255, 0, 0));
            paint1.setStyle(SkPaint::kFill_Style);

            paint2.setTextSize(64.f);
            paint2.setAntiAlias(true);
            paint2.setColor(SkColorSetRGB(0, 136, 0));
            paint2.setStyle(SkPaint::kStroke_Style);
            paint2.setStrokeWidth(SkIntToScalar(3));

            paint3.setTextSize(64.0f);
            paint3.setAntiAlias(true);
            paint3.setColor(SkColorSetRGB(136, 136, 136));
            paint3.setTextScaleX(SkFloatToScalar(1.5f));

            const char text[] = "Skia!";
            sk_canvas.drawText(text, strlen(text), 20.0f, 64.0f,  paint1);
            sk_canvas.drawText(text, strlen(text), 20.0f, 144.0f, paint2);
            sk_canvas.drawText(text, strlen(text), 20.0f, 224.0f, paint3);*/

            void* pBytes = sk_bitmap.getPixels();
            size_t size = sk_bitmap.getSize();
            _MirrorY(pBytes, sk_bitmap.width(), sk_bitmap.height());
            memcpy(bm.GetBits(), pBytes, sk_bitmap.getSize());
            /*Gdiplus::Image* pImageRes = RImagePool::GetInstance()->GetImage(m_sResourceFile);
            Gdiplus::Graphics g(dcMem);
            g.DrawImage(pImageRes, Gdiplus::Rect(0, 0, rc.Width(), rc.Height()), 0, 0, pImageRes->GetWidth(), pImageRes->GetHeight(), Gdiplus::UnitPixel);*/

        }

        /*RBitmap* myBitmap = RBitmapPool::GetInstance()->GetRItemBitmap(GetHItem());
        HBITMAP hOldBitmap = dcMem.SelectBitmap(*myBitmap);*/
        clock.Cal("draw text");

        BLENDFUNCTION Blend;
        Blend.BlendOp = 0;
        Blend.BlendFlags = 0;
        Blend.AlphaFormat = AC_SRC_ALPHA;
        Blend.SourceConstantAlpha = (BYTE)255;
        ::AlphaBlend(dc, rc.left, rc.top, rc.Width(), rc.Height(), dcMem, 0, 0, rc.Width(), rc.Height(), Blend);

        clock.Cal("alphablend");

        //dcMem.SelectBitmap(hOldBitmap);

        __super::Redraw(dc, invalidateRgn);
    }

    void RImage::_UpdateMyBitmap(HDC dc)
    {
        Gdiplus::Image* pImageRes = RImagePool::GetInstance()->GetImage(m_sResourceFile);
        if (!pImageRes)
            return;

        RBitmap* myBitmap = RBitmapPool::GetInstance()->GetRItemBitmap(GetHItem());
        if (!myBitmap)
            return;

        CRect rc;
        GetRect(rc);

        bool bNeedPaint = true;
        if (!myBitmap->IsValid())
        {
            myBitmap->CreateCompatibleBitmap(dc, rc.Width(), rc.Height());
        }
        else
        {
            BITMAP bitmap;
            myBitmap->GetBitmap(bitmap);
            if (bitmap.bmWidth == rc.Width() && bitmap.bmHeight == rc.Height())
                bNeedPaint = false;
        }
        if (bNeedPaint)
        {
            RDC dcMem;
            dcMem.CreateCompatibleDC(dc);
            HBITMAP hOldBitmap = dcMem.SelectBitmap(*myBitmap);

            //> rgn´¦Àí
            _CalRgn();
            HRGN hOldRgn = dcMem.SelectRgn(m_rgn);
            Gdiplus::Graphics g(dcMem);
            g.DrawImage(pImageRes, Gdiplus::Rect(0, 0, rc.Width(), rc.Height()), 0, 0, pImageRes->GetWidth(), pImageRes->GetHeight(), Gdiplus::UnitPixel);

            dcMem.SelectRgn(hOldRgn);
            dcMem.SelectBitmap(hOldBitmap);
        }
    }

    /*bool RImage::_CalAlignSize(CRect& rcParent)
    {
        return false;
    }*/

    void RImage::_MirrorY(LPVOID pBuff, int nWidth, int nHeight)
    {
        DWORD * pTop = (DWORD *)pBuff;
        DWORD * pBottom = (DWORD *)pBuff + nWidth * (nHeight - 1);
        while (pTop < pBottom)
        {
            DWORD * p1 = pTop;
            DWORD * p2 = pBottom;
            for(int i = 0; i < nWidth; i++)
            {
                DWORD dwTemp = *p1;
                *p1 = *p2;
                *p2 = dwTemp;
                p1++;
                p2++;
            }
            pTop += nWidth;
            pBottom -= nWidth;
        }
    }

}