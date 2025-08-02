#include "stdafx.h"
#include "NavigationBar.h"
#include "StackElement.h"

void UiElement::NavigationBar::Draw()
{
    CalculateRect();
    ui->DrawNavigationBar(rect, this);
    Element::Draw();
}

void UiElement::NavigationBar::LButtonUp(CPoint point)
{
    FindStackElement();
    if (stack_element != nullptr)
    {
        //���ҵ���ı�ǩ
        int _selected_index = -1;
        for (size_t i{}; i < item_rects.size(); i++)
        {
            if (item_rects[i].PtInRect(point))
            {
                _selected_index = i;
                break;
            }
        }
        if (_selected_index >= 0)
        {
            selected_index = _selected_index;
            stack_element->SetCurrentElement(selected_index);
        }
    }
}

void UiElement::NavigationBar::MouseMove(CPoint point)
{
    int _hover_index{ -1 };
    if (rect.PtInRect(point))
    {
        for (size_t i{}; i < item_rects.size(); i++)
        {
            if (item_rects[i].PtInRect(point))
            {
                _hover_index = i;
                break;
            }
        }
    }
    hover_index = _hover_index;

    //��ʾ�����ʾ
    if (icon_type == ICON_ONLY && hover_index >= 0)
    {
        if (last_hover_index != hover_index)
        {
            last_hover_index = hover_index;
            std::wstring str_tip = labels[hover_index];
            ui->UpdateMouseToolTip(TooltipIndex::TAB_ELEMENT, str_tip.c_str());
            ui->UpdateMouseToolTipPosition(TooltipIndex::TAB_ELEMENT, item_rects[hover_index]);
        }
    }
}

bool UiElement::NavigationBar::RButtunUp(CPoint point)
{
    //�������Ҽ��˵�
    return rect.PtInRect(point);
}

void UiElement::NavigationBar::MouseLeave()
{
    hover_index = -1;
}

int UiElement::NavigationBar::SelectedIndex()
{
    FindStackElement();
    if (stack_element != nullptr)
        return stack_element->GetCurIndex();
    else
        return selected_index;
}

void UiElement::NavigationBar::FindStackElement()
{
    if (!find_stack_element)
    {
        stack_element = FindRelatedElement<StackElement>();
        find_stack_element = true;  //�ҹ�һ��û�ҵ��Ͳ�����
    }
}
