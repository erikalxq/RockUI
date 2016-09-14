#include "stdafx.h"
#include "RockItem.h"
#include "RockItemPool.h"

namespace RockUI
{
    RItem::RItem(HRITEM hParentItem/* = NULL*/, HWND hContainer/* = NULL*/) 
    : m_id(0)
    , m_left(0)
    , m_top(0)
    , m_width(0)
    , m_height(0)
    , m_sAlign(c_align_fill)
    {
        m_hItem = RItemPool::GetInstance()->AddRItem(this);
        SetParent(hParentItem);
        SetContainer(hContainer);
    }

    RItem::~RItem()
    {
        RItemPool::GetInstance()->RemoveRItem(m_hItem);
    }

    void RItem::GetRect(CRect& rc)
    {
        rc = m_rcItem;
    }

    void RItem::SetParent(HRITEM hParentItem)
    {
        m_hParentItem = hParentItem;
    }

    void RItem::SetContainer(HWND hContainer)
    {
        m_hContainer = hContainer;
    }

    RockUI::HRITEM RItem::GetParent()
    {
        return m_hParentItem;
    }

    HWND RItem::GetContainer()
    {
        return m_hContainer;
    }

    RockUI::HRITEM RItem::GetHItem()
    {
        return m_hItem;
    }

    void RItem::Resize()
    {
        CRect rcParent;
        _GetParentRect(rcParent);

        if (!_CalFixSize(rcParent))
            _CalAlignSize(rcParent);
    }

    void RItem::Destroy()
    {

    }

    void RItem::Redraw(HDC dc, RRgn& invalidateRgn)
    {

    }

    void RItem::NotifyInvalidate()
    {
        HWND hContainer = GetContainer();
        ATLASSERT(hContainer);

        CRect rcItem;
        GetRect(rcItem);
        if (rcItem.IsRectNull() || rcItem.IsRectEmpty())
            return;

        ::SendMessage(hContainer, ROCKWM_INVALIDATE, (WPARAM)&rcItem, (LPARAM)m_hItem);
    }

    void RItem::NotifyRedraw()
    {
        HWND hContainer = GetContainer();
        ATLASSERT(hContainer);

        CRect rcItem;
        GetRect(rcItem);
        if (rcItem.IsRectNull() || rcItem.IsRectEmpty())
            return;

        ::SendMessage(hContainer, ROCKWM_REDRAW, (WPARAM)&rcItem, (LPARAM)m_hItem);
    }

    void RItem::SetWidth(int iWidth)
    {
        m_width = iWidth;
    }

    void RItem::SetHeight(int iHeight)
    {
        m_height = iHeight;
    }

    void RItem::SetLeft(int iLeft)
    {
        m_left = iLeft;
    }

    void RItem::SetTop(int iTop)
    {
        m_top = iTop;
    }

    void RItem::AddRgn(RRgn* pRgn)
    {
        m_vecRgn.push_back(pRgn);
    }

    bool RItem::_CalFixSize(CRect& rcParent)
    {
        bool bResult = false;
        //> 只处理设置了具体位置的，其余align情况派生类自行处理
        if ( m_left != 0 || m_top != 0 )
        {
            m_rcItem.left = rcParent.left + m_left;
            m_rcItem.top = rcParent.top + m_top;
            m_rcItem.right = m_rcItem.left + m_width;
            m_rcItem.bottom = m_rcItem.top + m_height;
            bResult = true;
        }
        return bResult;
    }

    bool RItem::_CalAlignSize(CRect& rcParent)
    {
        return false;
    }

    void RItem::_GetParentRect(CRect& rc)
    {
        ATLASSERT(m_hContainer);

        if (!m_hParentItem)
        {
            ::GetClientRect(m_hContainer, rc);
        }
        else
        {
            RItem* pParentItem = RItemPool::GetInstance()->FindRItem(m_hParentItem);
            ATLASSERT(pParentItem);
            pParentItem->GetRect(rc);
        }
    }

    void RItem::_CalRgn()
    {
        m_rgn.DeleteObject();

        CRect rc;
        GetRect(rc);
        rc.OffsetRect(-rc.left, -rc.top);
        m_rgn.CreateRectRgnIndirect(rc);

        for (std::vector<RRgn*>::iterator it = m_vecRgn.begin(); it != m_vecRgn.end(); ++it)
        {
            RRgn* pRgn = *it;
            m_rgn.CombineRgn(*pRgn, RGN_AND);
        }
    }

}