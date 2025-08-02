#include "stdafx.h"
#include "StackElement.h"

void UiElement::StackElement::SetCurrentElement(int index)
{
    if (index >= 0 && index < static_cast<int>(childLst.size()))
        cur_index = index;
    else
        index = 0;
}

void UiElement::StackElement::SwitchDisplay(bool previous)
{
    if (previous)
    {
        cur_index--;
        if (cur_index < 0)
            cur_index = static_cast<int>(childLst.size()) - 1;
    }
    else
    {
        cur_index++;
        if (cur_index >= static_cast<int>(childLst.size()))
            cur_index = 0;
    }
}

void UiElement::StackElement::Draw()
{
    auto cur_element{ CurrentElement() };

    //��ղ���ʾ����Ԫ�صľ�������
    for (size_t i{}; i < childLst.size(); i++)
    {
        if (cur_element != childLst[i])
        {
            childLst[i]->IterateAllElements([&](UiElement::Element* element) ->bool {
                if (element != nullptr)
                    element->ClearRect();
                return false;
                });
        }
    }

    if (cur_element != nullptr)
        cur_element->Draw();
    //ֻ����һ����Ԫ��
    //�����û����Draw������

    //����ָʾ��
    if (show_indicator)
    {
        //����ָʾ����λ��
        int indicator_width = ui->DPI(12) * childLst.size();
        indicator.rect.top = GetRect().bottom + ui->DPI(2) + ui->DPI(indicator_offset);
        indicator.rect.bottom = indicator.rect.top + ui->DPI(12);
        indicator.rect.left = GetRect().left + (GetRect().Width() - indicator_width) / 2;
        indicator.rect.right = indicator.rect.left + indicator_width;
        indicator.rect.InflateRect(ui->DPI(2), ui->DPI(2));
        //����ָʾ��
        ui->DrawStackIndicator(indicator, childLst.size(), cur_index);
    }
}

int UiElement::StackElement::GetCurIndex() const
{
    return cur_index;
}

std::shared_ptr<UiElement::Element> UiElement::StackElement::CurrentElement()
{
    if (hover_to_switch && mouse_hover)
    {
        int next_index = cur_index + 1;
        if (next_index < 0 || next_index >= static_cast<int>(childLst.size()))
            next_index = 0;
        return GetElement(next_index);
    }
    else
    {
        return GetElement(cur_index);
    }
}

std::shared_ptr<UiElement::Element> UiElement::StackElement::GetElement(int index)
{
    if (childLst.empty())
        return nullptr;
    else if (index >= 0 && index < static_cast<int>(childLst.size()))
        return childLst[index];
    else
        return childLst[0];
}