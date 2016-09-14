#ifndef __ROCKUI_ENV_H__
#define __ROCKUI_ENV_H__

namespace RockUI
{
    class RockUIEnv
    {
    public:
        RockUIEnv()
        {
            Gdiplus::GdiplusStartupInput gdiplusStartupInput;
            Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
        }
        ~RockUIEnv()
        {
            RImagePool::ReleaseInstance();
            RItemPool::ReleaseInstance();
            RBitmapPool::ReleaseInstance();
            Gdiplus::GdiplusShutdown(m_gdiplusToken);
        }

    private:
        ULONG_PTR m_gdiplusToken;
    };
}

#endif