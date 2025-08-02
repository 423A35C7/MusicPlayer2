#pragma once
#include "UIElement/UIElement.h"
namespace UiElement
{
    //ռλ��
    class PlaceHolder : public Element
    {
    public:
        virtual int GetWidth(CRect parent_rect) const override;
        virtual int GetHeight(CRect parent_rect) const override;
        virtual bool IsWidthValid() const override;

        bool show_when_use_system_titlebar{};   //����������ʹ��ϵͳ��׼��������ʱ����ʾ

    private:
        bool IsHide() const;
    };
}

