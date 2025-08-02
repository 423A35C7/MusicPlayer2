#pragma once
#include "UIElement/UIElement.h"
namespace UiElement
{
    //�ı�
    class Text : public Element
    {
    public:
        std::wstring text;
        Alignment align{};    //���뷽ʽ
        enum Style       //�ı�����ʽ
        {
            Static,     //��ֹ���ı�
            Scroll,     //�������ı�
            Scroll2     //��һ�ֹ������ı���ֻ��һ�����������
        };
        Style style;

        enum Type       //�ı�������
        {
            UserDefine, //�û�ָ����text��ֵ��
            Title,      //��������
            Artist,     //����������
            Album,      //������Ƭ��
            ArtistTitle,    //������ - ����
            ArtistAlbum,    //������ - ��Ƭ��
            Format,     //������ʽ
            PlayTime,   //����ʱ��
            PlayTimeAndVolume   //��ʾΪ����ʱ�䣬������ڵ�������������ʾ��ǰ������һ��ʱ���ָ�
        };

        Type type;
        int font_size{ 9 };
        bool width_follow_text{};
        CPlayerUIBase::ColorMode color_mode{ CPlayerUIBase::RCM_AUTO };
        bool show_volume{};     //��typeΪPlayTimeAndVolumeʱ��Ч�����Ϊtrue������ʾΪ����

        virtual void Draw() override;
        virtual int GetMaxWidth(CRect parent_rect) const override;
        std::wstring GetText() const;

    private:
        mutable CDrawCommon::ScrollInfo scroll_info;
    };
}

