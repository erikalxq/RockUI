#include "stdafx.h"
#include "RockLayer.h"

namespace RockUI
{
    RLayer::RLayer(HRITEM hParentItem /*= NULL*/, HWND hContainer /*= NULL*/) : RItem(hParentItem, hContainer)
    {

    }

    RLayer::~RLayer()
    {
        for (RItemSet::iterator itItem = m_subItemSet.begin(); itItem != m_subItemSet.end(); ++itItem)
        {
            HRITEM hItem = *itItem;
            RItem* pItem = RItemPool::GetInstance()->FindRItem(hItem);
            if (pItem)
            {
                delete pItem;
            }
        }
        m_subItemSet.clear();
    }

     void RLayer::Resize()
     {
         __super::Resize();
         _ResizeChildren();
     }

     void RLayer::Destroy()
     {
         for (RItemSet::iterator itItem = m_subItemSet.begin(); itItem != m_subItemSet.end(); ++itItem)
         {
             HRITEM hItem = *itItem;
             RItem* pItem = RItemPool::GetInstance()->FindRItem(hItem);
             if (pItem)
             {
                 pItem->Destroy();
             }
         }
     }

     void RLayer::Redraw(HDC dc, RRgn& invalidateRgn)
     {
         for (RItemSet::iterator itItem = m_subItemSet.begin(); itItem != m_subItemSet.end(); ++itItem)
         {
             RItem* pItem = RItemPool::GetInstance()->FindRItem(*itItem);
             if (!pItem)
                 continue;

             CRect rcItem;
             pItem->GetRect(rcItem);
             if (!invalidateRgn.RectInRegion(rcItem))
                 continue;

             pItem->Redraw(dc, invalidateRgn);
         }
     }

    bool RLayer::AttachChild(HRITEM hItem)
    {
        RItem* pItem = RItemPool::GetInstance()->FindRItem(hItem);
        if (pItem != NULL)
        {
            pItem->SetContainer(GetContainer());
            pItem->SetParent(GetHItem());
            m_subItemSet.insert(hItem);
            return true;
        }
        return false;
    }

    bool RLayer::DetachChild(HRITEM hItem)
    {
        m_subItemSet.erase(hItem);
        return true;
    }

    bool RLayer::_CalAlignSize(CRect& rcParent)
    {
        if (m_sAlign.compare(c_align_fill) == 0)
        {
            m_rcItem = rcParent;
        }

        if (m_left != 0 && m_left < (UINT)abs(rcParent.right))
        {
            m_rcItem.left = rcParent.left + m_left;
        }
        else if (m_top != 0 && m_top < (UINT)abs(rcParent.bottom))
        {
            m_rcItem.top = rcParent.top + m_top;
        }
        else if (m_width != 0 && m_width < (UINT)abs(rcParent.right - m_rcItem.left))
        {
            m_rcItem.right = m_rcItem.left + m_width;
        }
        else if (m_height != 0 && m_height < (UINT)abs(rcParent.bottom - m_rcItem.top))
        {
            m_rcItem.bottom = m_rcItem.top + m_height;
        }
        return true;
    }

    void RLayer::_ResizeChildren()
    {
        for (RItemSet::iterator itItem = m_subItemSet.begin(); itItem != m_subItemSet.end(); ++itItem)
        {
            HRITEM hItem = *itItem;
            RItem* pItem = RItemPool::GetInstance()->FindRItem(hItem);
            if (pItem)
                pItem->Resize();
        }
    }
}