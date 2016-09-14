#ifndef __ROCK_TEXT_H__
#define __ROCK_TEXT_H__

namespace RockUI
{
    class RText : public RItem
    {
    public:
        RText(HRITEM hParentItem = NULL, HWND hContainer = NULL);
        virtual ~RText(void);
    };
}

#endif