#include "stdafx.h"
#include "RockBitmapPool.h"

namespace RockUI
{
    __declspec(selectany) RBitmapPool* RBitmapPool::s_bitmapPool = NULL;

    RBitmapPool::RBitmapPool()
    {

    }

    RBitmapPool::~RBitmapPool()
    {
        for (RBitmapMap::iterator it = m_itemMap.begin(); it != m_itemMap.end(); ++it)
        {
            delete it->second;
        }
        m_itemMap.clear();
    }

    RBitmapPool* RBitmapPool::GetInstance()
    {
        if (!s_bitmapPool)
        {
            s_bitmapPool = new RBitmapPool;
        }
        return s_bitmapPool;
    }

    void RBitmapPool::ReleaseInstance()
    {
        if (s_bitmapPool)
        {
            delete s_bitmapPool;
            s_bitmapPool = NULL;
        }
    }


    RBitmap* RBitmapPool::GetRItemBitmap(HRITEM hItem)
    {
        RBitmap* pRbitmap = NULL;
        RBitmapMap::iterator it = m_itemMap.find(hItem);
        if (it == m_itemMap.end())
        {
            pRbitmap = new RBitmap;
            m_itemMap[hItem] = pRbitmap;
        }
        else
        {
            pRbitmap = it->second;
        }
        return pRbitmap;
    }

    void RBitmapPool::RemoveRItemBitmap(HRITEM hItem)
    {
        RBitmapMap::iterator it = m_itemMap.find(hItem);
        if (it == m_itemMap.end())
            return;

        delete it->second;
        m_itemMap.erase(it);
    }

}