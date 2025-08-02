#include "stdafx.h"
#include "SearchBox.h"
#include "UiSearchBox.h"
#include "ListElement.h"

UiElement::SearchBox::SearchBox()
{
}

UiElement::SearchBox::~SearchBox()
{
    CCommon::DeleteModelessDialog(search_box_ctrl);
}

void UiElement::SearchBox::InitSearchBoxControl(CWnd* pWnd)
{
    CCommon::DeleteModelessDialog(search_box_ctrl);
    search_box_ctrl = new CUiSearchBox(pWnd);
    search_box_ctrl->Create();
}

void UiElement::SearchBox::OnKeyWordsChanged()
{
    FindListElement();
    if (list_element != nullptr)
        list_element->QuickSearch(key_word);
}

void UiElement::SearchBox::Clear()
{
    search_box_ctrl->Clear();
}

void UiElement::SearchBox::Draw()
{
    CalculateRect();
    ui->DrawSearchBox(rect, this);
    Element::Draw();
}

void UiElement::SearchBox::MouseMove(CPoint point)
{
    hover = false;
    clear_btn.hover = false;
    //���ָ��ͼ������
    if (icon_rect.PtInRect(point))
    {
        clear_btn.hover = true;
        //���������ʾ
        if (!key_word.empty())
            ui->UpdateMouseToolTipPosition(TooltipIndex::SEARCHBOX_CLEAR_BTN, clear_btn.rect);
        else
            ui->UpdateMouseToolTipPosition(TooltipIndex::SEARCHBOX_CLEAR_BTN, CRect());
    }
    //ָ������������
    else if (rect.PtInRect(point))
    {
        hover = true;
    }
}

void UiElement::SearchBox::MouseLeave()
{
    hover = false;
    clear_btn.hover = false;
}

void UiElement::SearchBox::LButtonUp(CPoint point)
{
    clear_btn.pressed = false;
    //��������ťʱ����������
    if (icon_rect.PtInRect(point))
    {
        search_box_ctrl->Clear();
    }
    //�������������ʱ��ʾ������ؼ�
    else if (search_box_ctrl != nullptr && rect.PtInRect(point))
    {
        bool big_font{ ui->m_ui_data.full_screen && ui->IsDrawLargeIcon() };
        search_box_ctrl->Show(this, big_font);
    }
}

void UiElement::SearchBox::LButtonDown(CPoint point)
{
    if (icon_rect.PtInRect(point))
    {
        clear_btn.pressed = true;
    }
}

void UiElement::SearchBox::FindListElement()
{
    if (!find_list_element)
    {
        list_element = FindRelatedElement<ListElement>();
        if (list_element != nullptr)
            list_element->SetRelatedSearchBox(this);
        find_list_element = true;  //�ҹ�һ��û�ҵ��Ͳ�����
    }
}
