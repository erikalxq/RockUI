#ifndef __ROCK_IMAGE_H__
#define __ROCK_IMAGE_H__

namespace RockUI
{
    class RImage : public RLayer
    {
    public:
        RImage(HRITEM hParentItem = NULL, HWND hContainer = NULL);
        ~RImage();
        virtual void SetResource(std::wstring sResourceFile);
        virtual void Redraw(HDC dc, RRgn& invalidateRgn);

    protected:
        void _UpdateMyBitmap(HDC dc);
        void _MirrorY(LPVOID pBuff, int nWidth, int nHeight);
        //virtual bool _CalAlignSize(CRect& rcParent);

    private:
        std::wstring m_sResourceFile;
    };
}

#endif