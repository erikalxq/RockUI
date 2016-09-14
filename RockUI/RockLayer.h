#ifndef __ROCK_LAYER_H__
#define __ROCK_LAYER_H__

namespace RockUI
{
    class RLayer : public RItem
    {
    public:
        RLayer(HRITEM hParentItem = NULL, HWND hContainer = NULL);
        virtual ~RLayer();
        virtual void Resize();
        virtual void Destroy();
        virtual void Redraw(HDC dc, RRgn& invalidateRgn);
        bool AttachChild(HRITEM hItem);
        bool DetachChild(HRITEM hItem);

    protected:
        virtual bool _CalAlignSize(CRect& rcParent);
        void _ResizeChildren();

    private:
        RItemSet m_subItemSet;
    };
}

#endif