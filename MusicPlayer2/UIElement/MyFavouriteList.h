#pragma once
#include "ListElement.h"
namespace UiElement
{
    //��ϲ���������б�
    class MyFavouriteList : public ListElement
    {
    public:
        enum Column
        {
            COL_INDEX,
            COL_TRACK,
            COL_TIME,
            COL_MAX
        };

        //���ָ��һ��ʱ��ʾ�İ�ť
        enum BtnKey
        {
            BTN_PLAY,
            BTN_ADD,
            BTN_MAX
        };

        // ͨ�� ListElement �̳�
        std::wstring GetItemText(int row, int col) override;
        int GetRowCount() override;
        int GetColumnCount() override;
        int GetColumnWidth(int col, int total_width) override;
        virtual int GetHighlightRow() override;
        virtual int GetColumnScrollTextWhenSelected() override;
        virtual CMenu* GetContextMenu(bool item_selected) override;
        virtual void OnDoubleClicked() override;
        virtual std::wstring GetEmptyString() override;
        virtual int GetHoverButtonCount(int row) override;
        virtual int GetHoverButtonColumn() override;
        virtual IconMgr::IconType GetHoverButtonIcon(int index, int row) override;
        virtual std::wstring GetHoverButtonTooltip(int index, int row) override;
        virtual void OnHoverButtonClicked(int btn_index, int row) override;
        virtual bool IsMultipleSelectionEnable() override;
    };
}

