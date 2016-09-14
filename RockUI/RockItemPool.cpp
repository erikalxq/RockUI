#include "stdafx.h"
#include "RockItemPool.h"

namespace RockUI
{
    __declspec(selectany) RItemPool* RItemPool::s_itemPool = NULL;
    __declspec(selectany) HRITEM RItemPool::s_itemId = 0;

    RItemPool::RItemPool()
    {

    }

    RItemPool::~RItemPool()
    {

    }

    RItemPool* RItemPool::GetInstance()
    {
        if (!s_itemPool)
        {
            s_itemPool = new RItemPool;
        }
        return s_itemPool;
    }

    void RItemPool::ReleaseInstance()
    {
        if (s_itemPool)
        {
            delete s_itemPool;
            s_itemPool = NULL;
        }
    }

    DWORD RItemPool::AddRItem(RItem* pItem)
    {
        if (!pItem)
            return -1;

        RCSHelper scLock;

        s_itemId ++;
        m_itemMap[s_itemId] = pItem;

        return s_itemId;
    }

    RItem* RItemPool::FindRItem(DWORD hItem)
    {
        RCSHelper scLock;

        RItemMap::iterator it = m_itemMap.find(hItem);
        if (it != m_itemMap.end())
            return it->second;

        return NULL;
    }

    void RItemPool::RemoveRItem(DWORD hItem)
    {
        RCSHelper scLock;
        m_itemMap.erase(hItem);
    }

}