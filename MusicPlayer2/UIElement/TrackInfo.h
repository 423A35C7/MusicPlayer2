#pragma once
#include "UIElement/UIElement.h"
namespace UiElement
{
    //��Ŀ��Ϣ����������״̬���ļ�����������ʶ���ٶȣ�
    class TrackInfo : public Element
    {
    public:
        int font_size{ 9 };
        virtual void Draw() override;
    };
}

