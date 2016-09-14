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
            创建兼容性bitmap(DDB)，和传入的hDC同格式。
            1、新建的memory dc默认采用的是单色bitmap，所有不应该将该memory dc作为CreateCompatibleBitmap的HDC传入，除非你就是想用单色bitmap
            2、如果memory dc选择了DIBSection创建的bitmap，那么CreateCompatibleBitmap使用该dc的话，也会创建出DIBSection
            3、使用完需要DeleteObject
        */
        HBITMAP CreateCompatibleBitmap(HDC hDC, int iWidth, int iHeight);
        HBITMAP SmartCreateCompatibleBitmap(HDC hDC, int iWidth, int iHeight);

        //> 创建设备独立bitmap
        HBITMAP CreateDIBSection(int biWidth, int biHeight, int biPlanes = 1, int biBitCount = 32);

        //> 绑定新的bitmap，自身先DeleteObject，同时也会导致传入的bitmap执行detach
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