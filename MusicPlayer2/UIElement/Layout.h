#pragma once
#include "UIElement/UIElement.h"
namespace UiElement
{
    //����
    class Layout : public Element
    {
    public:
        enum Type
        {
            Vertical,
            Horizontal,
        };
        Type type;
        void CalculateChildrenRect();      //���㲼����������Ԫ�ص�λ��
        virtual void Draw() override;
    };

}

