#pragma once
#include "UIElement/UIElement.h"
namespace UiElement
{
    //������
    class Toolbar : public Element
    {
    public:
        bool show_translate_btn{};      //�Ƿ��ڹ���������ʾ����ʾ��ʷ��롱��ť
        virtual void Draw() override;
    };
}
