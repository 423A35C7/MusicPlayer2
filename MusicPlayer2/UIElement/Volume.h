#pragma once
#include "UIElement/UIElement.h"
namespace UiElement
{
    //����
    class Volume : public Element
    {
    public:
        bool show_text{ true };     //�Ƿ�������ͼ���Ա���ʾ�ı�
        bool adj_btn_on_top{ false };   //�������ڰ�ť�Ƿ���ʾ������ͼ����Ϸ�
        virtual void Draw() override;
    };
}

