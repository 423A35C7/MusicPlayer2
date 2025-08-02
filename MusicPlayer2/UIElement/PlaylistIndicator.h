#pragma once
#include "UIElement/UIElement.h"
namespace UiElement
{
    //��ǰ�����б�ָʾ
    class PlaylistIndicator : public Element
    {
    public:
        static CListCache m_list_cache;     // ΪPlaylistIndicator�Ļ��ƻ��浱ǰ���ŵ�ListItem��Draw֮ǰ����reload
        virtual void Draw() override;
        virtual void LButtonUp(CPoint point) override;
        virtual void LButtonDown(CPoint point) override;
        virtual void MouseMove(CPoint point) override;
        virtual void MouseLeave() override;
        virtual void ClearRect() override;

        int font_size{ 9 };

        IPlayerUI::UIButton btn_drop_down;
        IPlayerUI::UIButton btn_menu;
        CRect rect_name;
    };
}

