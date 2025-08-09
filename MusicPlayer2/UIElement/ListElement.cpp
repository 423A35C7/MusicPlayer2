#include "stdafx.h"
#include "ListElement.h"
#include "UIWindowCmdHelper.h"
#include "SearchBox.h"

void UiElement::ListElement::Draw()
{
    CalculateRect();
    RestrictOffset();
    CalculateItemRects();

    if (last_row_count != GetRowCount())
    {
        OnRowCountChanged();
        last_row_count = GetRowCount();
    }

    ui->DrawList(rect, this, ItemHeight());
    Element::Draw();
}

void UiElement::ListElement::LButtonUp(CPoint point)
{
    mouse_pressed = false;
    scrollbar_handle_pressed = false;
    int row{ GetListIndexByPoint(point) };        //�������
    //���ð�ť�İ���״̬
    for (int i{}; i < GetHoverButtonCount(row); i++)
    {
        auto& btn{ GetHoverButtonState(i) };
        if (btn.pressed)
        {
            if (btn.rect.PtInRect(point))
                OnHoverButtonClicked(i, row);
            btn.pressed = false;
        }
    }
}

void UiElement::ListElement::LButtonDown(CPoint point)
{
    //����˿ؼ�����
    if (rect.PtInRect(point))
    {
        //����˹���������
        if (scrollbar_rect.PtInRect(point))
        {
            //����˹�������������
            if (scrollbar_handle_rect.PtInRect(point))
            {
                scrollbar_handle_pressed = true;
            }
            //����˹������հ�����
            else
            {
                mouse_pressed = false;
            }
        }
        //������б�����
        else
        {
            int clicked_index{ GetListIndexByPoint(point) };        //�������
            //���ð�ť�İ���״̬
            for (int i{}; i < GetHoverButtonCount(clicked_index); i++)
            {
                auto& btn{ GetHoverButtonState(i) };
                btn.pressed = btn.rect.PtInRect(point);
            }

            //�����ѡʱ
            if (IsMultipleSelectionEnable())
            {
                //�Ƿ���Ctrl��
                if (GetKeyState(VK_CONTROL) & 0x80)
                {
                    if (items_selected.contains(clicked_index))
                        items_selected.erase(clicked_index);
                    else
                        items_selected.insert(clicked_index);
                }
                //�Ƿ���Shift������������ѡ����һ��
                else if (GetKeyState(VK_SHIFT) & 0x8000 && !items_selected.empty())
                {
                    int first_selected = *items_selected.begin();   //ѡ�еĵ�һ��
                    items_selected.clear();
                    //���������ѡ�еĵ�һ�к���
                    if (first_selected < clicked_index)
                    {
                        for (int i = first_selected; i <= clicked_index; i++)
                            items_selected.insert(i);
                    }
                    //���������ѡ�еĵ�һ��ǰ��
                    else
                    {
                        for (int i = clicked_index; i <= first_selected; i++)
                            items_selected.insert(i);
                    }
                }
                else
                {
                    SetItemSelected(clicked_index);
                }
            }
            //����ѡʱ
            else
            {
                SetItemSelected(clicked_index);
            }
            OnClicked();
            selected_item_scroll_info.Reset();
            mouse_pressed = true;
        }
        mouse_pressed_offset = playlist_offset;
        mouse_pressed_pos = point;
    }
    //����˿ؼ���
    else
    {
        mouse_pressed = false;
        //item_selected = -1;
    }
}

void UiElement::ListElement::MouseMove(CPoint point)
{
    if (rect.IsRectEmpty())
        return;

    mouse_pos = point;
    hover = rect.PtInRect(point);
    scrollbar_hover = scrollbar_rect.PtInRect(point);
    if (scrollbar_handle_pressed)
    {
        int delta_scrollbar_offset = mouse_pressed_pos.y - point.y;  //�������ƶ��ľ���
        //���������ƶ��ľ���ת���ɲ����б��λ��
        int scroll_area_height = rect.Height() - scroll_handle_length_comp;
        if (scroll_area_height > 0)
        {
            int delta_playlist_offset = delta_scrollbar_offset * (ItemHeight() * GetDisplayRowCount()) / scroll_area_height;
            playlist_offset = mouse_pressed_offset - delta_playlist_offset;
        }
    }
    else if (mouse_pressed)
    {
        playlist_offset = mouse_pressed_offset + (mouse_pressed_pos.y - point.y);
    }

    //�������ָ�����
    int row = GetListIndexByPoint(point);

    //�����ʾ�˰�ť
    bool mouse_in_btn{ false };
    if (GetHoverButtonCount(row) > 0)
    {
        for (int i{}; i < GetHoverButtonCount(row); i++)
        {
            auto& btn{ GetHoverButtonState(i) };
            if (btn.rect.PtInRect(point) && rect.PtInRect(point))
            {
                mouse_in_btn = true;
                btn.hover = true;
                static int last_row{ -1 };
                static int last_btn_index{ -1 };
                if (last_row != row || last_btn_index != i)
                {
                    std::wstring btn_tooltip{ GetHoverButtonTooltip(i, row) };
                    ui->UpdateMouseToolTip(GetToolTipIndex(), btn_tooltip.c_str());
                    ui->UpdateMouseToolTipPosition(GetToolTipIndex(), btn.rect);
                }
                last_row = row;
                last_btn_index = i;
            }
            else
            {
                btn.hover = false;
            }
        }
    }

    //��ʾ�����ʾ
    if (!mouse_in_btn && ShowTooltip() && hover && !scrollbar_hover && !scrollbar_handle_pressed)
    {
        if (row >= 0)
        {
            static int last_row{ -1 };
            if (last_row != row)
            {
                last_row = row;
                std::wstring str_tip = GetToolTipText(row);

                ui->UpdateMouseToolTip(GetToolTipIndex(), str_tip.c_str());
                int display_row = row;
                AbsoluteRowToDisplayRow(display_row);
                if (display_row >= 0 && display_row < static_cast<int>(item_rects.size()))
                    ui->UpdateMouseToolTipPosition(GetToolTipIndex(), item_rects[display_row]);
            }
        }
    }
}

bool UiElement::ListElement::RButtunUp(CPoint point)
{
    if (rect.PtInRect(point))
    {
        mouse_pressed = false;
        CMenu* menu{ GetContextMenu(GetItemSelected() >= 0 && !scrollbar_rect.PtInRect(point)) };
        ShowContextMenu(menu, GetCmdRecivedWnd());
        return true;
    }
    return false;
}

void UiElement::ListElement::ShowContextMenu(CMenu* menu, CWnd* cmd_reciver)
{
    if (menu != nullptr)
    {
        CPoint cursor_pos;
        GetCursorPos(&cursor_pos);
        if (cmd_reciver != nullptr)
        {
            //�����Ҽ��˵�����ѡ����һ���˵�����ʱ��cmd_reciver����WM_COMMAND��Ϣ
            menu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, cursor_pos.x, cursor_pos.y, cmd_reciver);
        }
        else
        {
            CUIWindowCmdHelper helper(this);
            helper.SetMenuState(menu);
            //ʹ��TPM_RETURNCMD��־ָ���˵�����ʹ�÷���ֵ���أ�TPM_NONOTIFY��־ָ��ѡ���˲˵�����󲻻��򴰿ڷ���WM_COMMAND��Ϣ��������Ȼ���봫��һ����Ч�Ĵ��ھ��
            UINT command = menu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, cursor_pos.x, cursor_pos.y, theApp.m_pMainWnd);
            helper.OnUiCommand(command);
        }
    }
}


void UiElement::ListElement::RButtonDown(CPoint point)
{
    mouse_pressed = false;
    if (rect.PtInRect(point))
    {
        if (!scrollbar_rect.PtInRect(point))
        {
            int clicked_index{ GetListIndexByPoint(point) };        //�������
            if (!IsItemSelected(clicked_index))
            {
                SetItemSelected(clicked_index);
                OnClicked();
            }
            selected_item_scroll_info.Reset();
        }
    }
    else
    {
        items_selected.clear();
    }
}

bool UiElement::ListElement::MouseWheel(int delta, CPoint point)
{
    if (rect.PtInRect(point))
    {
        //һ�ι���������
        int lines = rect.Height() / ItemHeight() / 2;
        if (lines > 3)
            lines = 3;
        if (lines < 1)
            lines = 1;
        playlist_offset += (-delta * lines * ItemHeight() / 120);  //120Ϊ������һ��ʱdelta��ֵ
        return true;
    }
    return false;
}

void UiElement::ListElement::MouseLeave()
{
    hover = false;
    mouse_pressed = false;
    scrollbar_hover = false;
    scrollbar_handle_pressed = false;
}

bool UiElement::ListElement::DoubleClick(CPoint point)
{
    if (rect.PtInRect(point) && !scrollbar_rect.PtInRect(point) && GetItemSelected() >= 0)
    {
        OnDoubleClicked();
    }
    return false;
}

void UiElement::ListElement::ClearRect()
{
    Element::ClearRect();
    for (auto& btn : hover_buttons)
        btn.second.rect = CRect();
}

void UiElement::ListElement::EnsureItemVisible(int index)
{
    if (index <= 0)
    {
        playlist_offset = 0;
        return;
    }

    CalculateRect();
    CalculateItemRects();

    AbsoluteRowToDisplayRow(index);
    if (index < 0 || index >= static_cast<int>(item_rects.size()))
        return;

    CRect item_rect{ item_rects[index] };
    //ȷ����ǰ��Ŀ�Ƿ��ڿɼ�״̬
    if (item_rect.top > rect.top && item_rect.bottom < rect.bottom)
        return;

    //����Ҫʹָ����ɼ�ʱ��ƫ����
    int delta_offset{};
    //ָ����Ŀ�ڲ����б��Ϸ�
    if (item_rect.top < rect.top)
        delta_offset = rect.top - item_rect.top;
    //ָ����Ŀ�ڲ����б��·�
    else if (item_rect.bottom > rect.bottom)
        delta_offset = rect.bottom - item_rect.bottom;
    playlist_offset -= delta_offset;
}

void UiElement::ListElement::EnsureHighlightItemVisible()
{
    int highlight_row{ GetHighlightRow() };
    if (highlight_row >= 0)
        EnsureItemVisible(highlight_row);
}

void UiElement::ListElement::RestrictOffset()
{
    int& offset{ playlist_offset };
    if (offset < 0)
        offset = 0;
    int offset_max{ ItemHeight() * GetDisplayRowCount() - rect.Height() };
    if (offset_max <= 0)
        offset = 0;
    else if (offset > offset_max)
        offset = offset_max;
}

void UiElement::ListElement::CalculateItemRects()
{
    item_rects.resize(GetRowCount());
    for (size_t i{}; i < item_rects.size(); i++)
    {
        //����ÿһ�еľ�������
        int start_y = -playlist_offset + rect.top + i * ItemHeight();
        CRect rect_item{ rect };
        rect_item.top = start_y;
        rect_item.bottom = rect_item.top + ItemHeight();

        //����ÿһ�еľ�������
        item_rects[i] = rect_item;
    }
}

int UiElement::ListElement::ItemHeight() const
{
    return ui->DPI(item_height);
}

void UiElement::ListElement::SetItemSelected(int index)
{
    items_selected.clear();
    if (index >= 0)
    {
        items_selected.insert(index);
        EnsureItemVisible(index);
    }
}

int UiElement::ListElement::GetItemSelected() const
{
    if (!items_selected.empty())
        return *items_selected.begin();
    return -1;
}

void UiElement::ListElement::SetItemsSelected(const vector<int>& indexes)
{
    items_selected.clear();
    for (int index : indexes)
        items_selected.insert(index);
}

void UiElement::ListElement::GetItemsSelected(vector<int>& indexes) const
{
    indexes.clear();
    for (int index : items_selected)
        indexes.push_back(index);
}

bool UiElement::ListElement::IsItemSelected(int index) const
{
    auto iter = std::find(items_selected.begin(), items_selected.end(), index);
    return iter != items_selected.end();
}

bool UiElement::ListElement::IsMultipleSelected() const
{
    return items_selected.size() > 1;
}

void UiElement::ListElement::SelectAll()
{
    if (IsMultipleSelectionEnable())
    {
        items_selected.clear();
        for (int i{}; i < GetRowCount(); i++)
            items_selected.insert(i);
    }
}

void UiElement::ListElement::SelectNone()
{
    items_selected.clear();
}

void UiElement::ListElement::SelectReversed()
{
    if (IsMultipleSelectionEnable())
    {
        auto items_selected_old{ items_selected };
        items_selected.clear();
        for (int i{}; i < GetRowCount(); i++)
        {
            if (!items_selected_old.contains(i))
                items_selected.insert(i);
        }
    }
}

void UiElement::ListElement::DrawHoverButton(int index, int row)
{
    CRect rc_button = GetHoverButtonState(index).rect;
    ui->DrawUIButton(rc_button, GetHoverButtonState(index), GetHoverButtonIcon(index, row));
}

IPlayerUI::UIButton& UiElement::ListElement::GetHoverButtonState(int btn_index)
{
    return hover_buttons[btn_index];
}

void UiElement::ListElement::DrawUnHoverButton(CRect rc_button, int index, int row)
{
    ui->DrawUiIcon(rc_button, GetUnHoverIcon(index, row));
}

void UiElement::ListElement::OnRowCountChanged()
{
    //����б�������б仯�������ѡ��
    SelectNone();
    //���������
    if (related_search_box != nullptr)
        related_search_box->Clear();
}

void UiElement::ListElement::QuickSearch(const std::wstring& key_word)
{
    searched = !key_word.empty();

    //����ƥ������
    search_result.clear();
    if (key_word.empty())
        return;
    for (int i = 0; i < GetRowCount(); i++)
    {
        if (IsItemMatchKeyWord(i, key_word))
            search_result.push_back(i);
    }
}

bool UiElement::ListElement::IsItemMatchKeyWord(int row, const std::wstring& key_word)
{
    bool rtn = false;
    //Ĭ��ƥ��ÿһ���е��ı�
    for (int i = 0; i < GetColumnCount(); i++)
    {
        std::wstring text = GetItemText(row, i);
        if (!text.empty() && theApp.m_chinese_pingyin_res.IsStringMatchWithPingyin(key_word, text))
            return true;
    }
    return false;
}

int UiElement::ListElement::GetDisplayRowCount()
{
    if (searched)
        return search_result.size();
    else
        return GetRowCount();
}

bool UiElement::ListElement::IsRowDisplayed(int row)
{
    if (row >= 0 && row < GetRowCount())
    {
        //����״̬�£�����������е�����ʾ
        if (searched)
        {
            return CCommon::IsItemInVector(search_result, row);
        }
        //������״̬�£������ж���ʾ
        else
        {
            return true;
        }
    }
    return false;
}

void UiElement::ListElement::DisplayRowToAbsoluteRow(int& row)
{
    if (searched)       //����״̬����Ҫת���к�
    {
        if (row >= 0 && row < static_cast<int>(search_result.size()))
            row = search_result[row];
        else
            row = -1;
    }
}

void UiElement::ListElement::AbsoluteRowToDisplayRow(int& row)
{
    if (searched)       //����״̬����Ҫת���к�
    {
        bool row_exist{};
        for (int i{}; i < static_cast<int>(search_result.size()); i++)
        {
            if (row == search_result[i])
            {
                row = i;
                row_exist = true;
                break;
            }
        }
        if (!row_exist)
            row = -1;
    }
}

int UiElement::ListElement::GetListIndexByPoint(CPoint point)
{
    int index = GetDisplayedIndexByPoint(point);
    DisplayRowToAbsoluteRow(index);
    return index;
}

int UiElement::ListElement::GetDisplayedIndexByPoint(CPoint point)
{
    for (size_t i{}; i < item_rects.size(); i++)
    {
        if (item_rects[i].PtInRect(point))
            return static_cast<int>(i);
    }
    return -1;
}
