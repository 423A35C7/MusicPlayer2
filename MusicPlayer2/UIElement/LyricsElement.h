#pragma once
#include "UIElement/UIElement.h"
namespace UiElement
{
    //���
    class Lyrics : public Element
    {
    public:
        bool no_background = false;         // ���ǲ�ʹ�ø�ʱ���
        bool use_default_font = false;      // �̶�ʹ��Ĭ������
        int font_size{ 9 };                 // ʹ��Ĭ������ʱ���ֺ�
        bool show_song_info = false;        //û�и��ʱ������ʾ������Ϣ
        virtual void Draw() override;
        virtual void ClearRect() override;
    protected:
        bool IsParentRectangle() const;     //�жϸ�Ԫ�����Ƿ��о���Ԫ��
    };
}

