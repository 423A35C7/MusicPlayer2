#include "stdafx.h"
#include "PlaceHolder.h"

int UiElement::PlaceHolder::GetWidth(CRect parent_rect) const
{
    if (IsHide())
    {
        return 0;
    }
    else
    {
        if (show_when_use_system_titlebar)
            return ui->TopRightButtonsWidth();
        else
            return Element::GetWidth(parent_rect);
    }
}

int UiElement::PlaceHolder::GetHeight(CRect parent_rect) const
{
    if (IsHide())
        return 0;
    else
        return Element::GetHeight(parent_rect);
}

bool UiElement::PlaceHolder::IsWidthValid() const
{
    if (show_when_use_system_titlebar)
        return true;
    return Element::IsWidthValid();
}

bool UiElement::PlaceHolder::IsHide() const
{
    //�������е�ͼ�������С�������/��ԭ���Ƿ�����
    bool is_all_titlebar_icon_hide = !theApp.m_app_setting_data.show_minimode_btn_in_titlebar
        && !theApp.m_app_setting_data.show_fullscreen_btn_in_titlebar
        && !theApp.m_app_setting_data.show_skin_btn_in_titlebar
        && !theApp.m_app_setting_data.show_settings_btn_in_titlebar
        && !theApp.m_app_setting_data.show_dark_light_btn_in_titlebar
        ;
    //��������ˡ�����ʹ��ϵͳ��׼������ʱ����ʾ��������û��ʹ��ϵͳ��׼������������ʾ
    return (show_when_use_system_titlebar
        && !theApp.m_app_setting_data.show_window_frame
        && (!theApp.m_ui_data.full_screen) || (theApp.m_ui_data.show_menu_bar && is_all_titlebar_icon_hide));
}
