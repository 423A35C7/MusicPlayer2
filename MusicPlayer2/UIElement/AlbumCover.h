#pragma once
#include "UIElement/UIElement.h"
namespace UiElement
{
    //ר������
    class AlbumCover : public Element
    {
    public:
        bool square{};
        bool show_info{};
        virtual void Draw() override;
        virtual void CalculateRect() override;
    };
}

