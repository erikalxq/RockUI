#ifndef __ROCK_IMAGE_POOL_H__
#define __ROCK_IMAGE_POOL_H__

namespace RockUI
{
    typedef std::map<std::wstring, Gdiplus::Image*> RImageMap;
    typedef std::set<std::wstring> RResourcePathSet;

    class RImagePool
    {
    public:
        ~RImagePool();
        static RImagePool* GetInstance();
        static void ReleaseInstance();
        Gdiplus::Image* GetImage(std::wstring& sFile);
        void AddResourcePath(std::wstring sPath);

    protected:
        RImagePool();
        RImagePool(const RImagePool& pool);
        RImagePool& operator = (const RImagePool& pool);

    protected:
        void _clear();
        Gdiplus::Image* _GetImage(std::wstring& sPath, std::wstring& sFile);

    private:
        static RImagePool* s_imagePool;
        RImageMap m_imageMap;
        RResourcePathSet m_resourcePathSet;
    };
}


#endif