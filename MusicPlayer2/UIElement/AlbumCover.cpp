#include "stdafx.h"
#include "AlbumCover.h"
void UiElement::AlbumCover::Draw()
{
    CalculateRect();
    if (show_info)
        ui->DrawAlbumCoverWithInfo(rect);
    else
        ui->DrawAlbumCover(rect);
    Element::Draw();
}

void UiElement::AlbumCover::CalculateRect()
{
    Element::CalculateRect();
    CRect cover_rect{ rect };
    //���ǿ��ר������Ϊ�����Σ�������������µľ�������
    if (square)
    {
        int side{ min(rect.Width(), rect.Height()) };
        if (rect.Width() > rect.Height())
        {
            cover_rect.left = rect.left + (rect.Width() - side) / 2;
            cover_rect.right = cover_rect.left + side;
        }
        else if (rect.Width() < rect.Height())
        {
            cover_rect.top = rect.top + (rect.Height() - side) / 2;
            cover_rect.bottom = cover_rect.top + side;
        }
        rect = cover_rect;
    }
}
