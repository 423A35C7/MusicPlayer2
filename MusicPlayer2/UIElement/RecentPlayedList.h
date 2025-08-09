#pragma once
#include "ListElement.h"
namespace UiElement
{
    //�������
    class RecentPlayedList : public ListElement
    {
    public:
        enum Column
        {
            COL_NAME,
            COL_COUNT,
            COL_MAX
        };
        static CListCache m_list_cache;     // ΪRecentPlayedList�Ļ��ƻ���������ŵ�ListItem��Draw֮ǰ����reload
        virtual void Draw() override;

        // ͨ�� ListElement �̳�
        std::wstring GetItemText(int row, int col) override;
        int GetRowCount() override;
        int GetColumnCount() override;
        int GetColumnWidth(int col, int total_width) override;
        virtual int GetColumnScrollTextWhenSelected() override;
        virtual IconMgr::IconType GetIcon(int row) override;
        virtual bool HasIcon() override;
        virtual void OnDoubleClicked() override;
        virtual CMenu* GetContextMenu(bool item_selected) override;
        virtual int GetHoverButtonCount(int row) override;
        virtual int GetHoverButtonColumn() override;
        virtual IconMgr::IconType GetHoverButtonIcon(int index, int row) override;
        virtual std::wstring GetHoverButtonTooltip(int index, int row) override;
        virtual void OnHoverButtonClicked(int btn_index, int row) override;
    };
}

