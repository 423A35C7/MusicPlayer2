#pragma once
#include "UIElement/UIElement.h"
namespace UiElement
{
    //Ƶ�׷���
    class Spectrum : public Element
    {
    public:
        bool draw_reflex{};     //�Ƿ���Ƶ�Ӱ
        bool fixed_width{};     //ÿ�������Ƿ�ʹ����ͬ�Ŀ��
        Alignment align{ Alignment::LEFT };     //���뷽ʽ
        CUIDrawer::SpectrumCol type{ CUIDrawer::SC_64 };     //Ƶ�׷���������
        virtual void Draw() override;
        virtual bool IsEnable(CRect parent_rect) const override;
    };
}

