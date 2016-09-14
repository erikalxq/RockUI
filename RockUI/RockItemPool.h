#ifndef __ROCK_ITEM_POOL_H__
#define __ROCK_ITEM_POOL_H__

namespace RockUI
{
    typedef std::map<DWORD, RItem*> RItemMap;
    class RItemPool
    {
    public:
        ~RItemPool();
        static RItemPool* GetInstance();
        static void ReleaseInstance();
        DWORD AddRItem(RItem* pItem);
        RItem* FindRItem(DWORD hItem);
        void RemoveRItem(DWORD hItem);

    protected:
        RItemPool();
        RItemPool(const RItemPool& pool);
        RItemPool& operator = (const RItemPool& pool);

    private:
        static RItemPool* s_itemPool;
        static HRITEM s_itemId;
        RItemMap m_itemMap;
    };
}

#endif