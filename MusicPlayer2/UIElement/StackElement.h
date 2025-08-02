#pragma once
#include "UIElement/UIElement.h"
namespace UiElement
{
    //�������Ԫ�صĶѵ�Ԫ�أ�ͬʱֻ����ʾһ��Ԫ��
    class StackElement : public Element
    {
    public:
        void SetCurrentElement(int index);
        void SwitchDisplay(bool previous = false);
        virtual void Draw() override;
        bool click_to_switch{};     //�����ʱ�л�
        bool hover_to_switch{};     //���ָ��ʱ�л�
        bool scroll_to_switch{};	//�������л�
        bool show_indicator{};
        int indicator_offset{};
        bool mouse_hover{};
        IPlayerUI::UIButton indicator{};        //ָʾ��
        int GetCurIndex() const;

    protected:
        std::shared_ptr<Element> CurrentElement();
        std::shared_ptr<Element> GetElement(int index);

        int cur_index{};
    };
}

