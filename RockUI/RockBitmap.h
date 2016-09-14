#ifndef __ROCK_BITMAP_H__
#define __ROCK_BITMAP_H__

namespace RockUI
{
    class RBitmap
    {
    public:
        RBitmap();
        RBitmap(HBITMAP hBitmap);
        virtual ~RBitmap();
        operator HBITMAP() const {return m_hBitmap;}
        bool DeleteObject();
        bool IsValid();
        BYTE* GetBits(){return m_bytes;}
        bool GetBitmap(BITMAP& pBitMap);
        bool CleanAllPix();
        bool CleanRgnPix(RRgn& rgn);
        /*
            ����������bitmap(DDB)���ʹ����hDCͬ��ʽ��
            1���½���memory dcĬ�ϲ��õ��ǵ�ɫbitmap�����в�Ӧ�ý���memory dc��ΪCreateCompatibleBitmap��HDC���룬������������õ�ɫbitmap
            2�����memory dcѡ����DIBSection������bitmap����ôCreateCompatibleBitmapʹ�ø�dc�Ļ���Ҳ�ᴴ����DIBSection
            3��ʹ������ҪDeleteObject
        */
        HBITMAP CreateCompatibleBitmap(HDC hDC, int iWidth, int iHeight);
        HBITMAP SmartCreateCompatibleBitmap(HDC hDC, int iWidth, int iHeight);

        //> �����豸����bitmap
        HBITMAP CreateDIBSection(int biWidth, int biHeight, int biPlanes = 1, int biBitCount = 32);

        //> ���µ�bitmap��������DeleteObject��ͬʱҲ�ᵼ�´����bitmapִ��detach
        void Attach(RBitmap& bitmap, bool bOwned = false);
        void Attach(HBITMAP hBitmap, bool bOwned = false);
        HBITMAP Detach();

    private:
        HBITMAP m_hBitmap;
        bool m_bOwned;
        BYTE* m_bytes;
    };
}

#endif