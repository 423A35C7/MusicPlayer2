#pragma once
#include "UIElement/UIElement.h"
namespace UiElement
{
    //������
    class ProgressBar : public Element
    {
    public:
        bool show_play_time{};
        bool play_time_both_side{};
        virtual void Draw() override;
    };
}

