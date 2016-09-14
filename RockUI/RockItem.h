#ifndef __ROCK_ITEM_H__
#define __ROCK_ITEM_H__

namespace RockUI
{
    class RItem
    {
    public:
        RItem(HRITEM hParentItem = NULL, HWND hContainer = NULL);
        virtual ~RItem();

        virtual void GetRect(CRect& rc);
        virtual void SetParent(HRITEM hParentItem);
        virtual void SetContainer(HWND hContainer);
        virtual HRITEM GetParent();
        virtual HWND GetContainer();
        virtual HRITEM GetHItem();
        virtual void Resize();
        virtual void Destroy();
        virtual void Redraw(HDC dc, RRgn& invalidateRgn);
        virtual void NotifyInvalidate();
        virtual void NotifyRedraw();
        virtual void SetWidth(int iWidth);
        virtual void SetHeight(int iHeight);
        virtual void SetLeft(int iLeft);
        virtual void SetTop(int iTop);
        virtual void AddRgn(RRgn* pRgn);

    protected:
        virtual bool _CalFixSize(CRect& rcParent);
        virtual bool _CalAlignSize(CRect& rcParent);
        void _GetParentRect(CRect& rc);
        void _CalRgn();

    protected:
        CRect m_rcItem;
        UINT m_left;
        UINT m_top;
        UINT m_width;
        UINT m_height;
        std::string m_sAlign;
        RRgn m_rgn;
        std::vector<RRgn*> m_vecRgn;

    private:
        UINT m_id;
        HRITEM m_hItem;
        HRITEM m_hParentItem;
        HWND m_hContainer;
    };

    typedef std::set<HRITEM> RItemSet;
}

#endif