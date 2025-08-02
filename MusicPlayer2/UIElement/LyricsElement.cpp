#include "stdafx.h"
#include "LyricsElement.h"
#include "Rectangle.h"
void UiElement::Lyrics::Draw()
{
    CalculateRect();

    bool big_font{ ui->m_ui_data.full_screen && ui->IsDrawLargeIcon() };
    CFont* lyric_font = &theApp.m_font_set.lyric.GetFont(big_font);
    CFont* lyric_tr_font = &theApp.m_font_set.lyric_translate.GetFont(big_font);

    if (use_default_font)   // Ŀǰ���bool��Щ���࣬���������ֺ���m_font_set�н��������
    {
        lyric_font = &theApp.m_font_set.GetFontBySize(font_size).GetFont(big_font);
        lyric_tr_font = &theApp.m_font_set.GetFontBySize(font_size - 1).GetFont(big_font);
    }

    //�����Ԫ���а����˾���Ԫ�أ���ʹ�ڡ�������á��й�ѡ�ˡ���ʽ��汳������Ҳ����Ϊ���������ư�͸������
    ui->DrawLyrics(rect, lyric_font, lyric_tr_font, (!no_background && !IsParentRectangle()), show_song_info);

    ui->m_draw_data.lyric_rect = rect;
    Element::Draw();
}

void UiElement::Lyrics::ClearRect()
{
    Element::ClearRect();
    ui->m_draw_data.lyric_rect = CRect();
}

bool UiElement::Lyrics::IsParentRectangle() const
{
    const Element* ele{ this };
    while (ele != nullptr && ele->pParent != nullptr)
    {
        if (dynamic_cast<const Rectangle*>(ele) != nullptr)
            return true;
        ele = ele->pParent;
    }
    return false;
}
