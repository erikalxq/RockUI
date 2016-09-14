#ifndef __ROCKBITMAP_POOL_H__
#define __ROCKBITMAP_POOL_H__

namespace RockUI
{
    typedef std::map<DWORD, RBitmap*> RBitmapMap;
    class RBitmapPool
    {
    public:
        ~RBitmapPool();
        static RBitmapPool* GetInstance();
        static void ReleaseInstance();
        RBitmap* GetRItemBitmap(HRITEM hItem);
        void RemoveRItemBitmap(HRITEM hItem);

    protected:
        RBitmapPool();
        RBitmapPool(const RBitmapPool& pool);
        RBitmapPool& operator = (const RBitmapPool& pool);

    private:
        static RBitmapPool* s_bitmapPool;
        RBitmapMap m_itemMap;
    };
}

#endif