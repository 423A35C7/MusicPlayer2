#pragma once
#include "UIElement/ListElement.h"
namespace UiElement
{
    //�����б�
    class Playlist : public ListElement
    {
    public:
        enum Column
        {
            COL_INDEX,
            COL_TRACK,
            COL_TIME,
            COL_MAX
        };

        ////���ָ��һ��ʱ��ʾ�İ�ť
        //enum BtnKey
        //{
        //    BTN_PLAY,
        //    BTN_ADD,
        //    BTN_FAVOURITE,
        //    BTN_MAX
        //};


        // ͨ�� ListElement �̳�
        std::wstring GetItemText(int row, int col) override;
        int GetRowCount() override;
        int GetColumnCount() override;
        virtual int GetColumnWidth(int col, int total_width) override;
        virtual std::wstring GetEmptyString() override;
        virtual int GetHighlightRow() override;
        virtual int GetColumnScrollTextWhenSelected() override;
        virtual bool ShowTooltip() override;
        virtual std::wstring GetToolTipText(int row) override;
        virtual int GetToolTipIndex() const override;
        virtual CMenu* GetContextMenu(bool item_selected) override;
        virtual CWnd* GetCmdRecivedWnd() override;
        virtual void OnDoubleClicked() override;
        virtual void OnClicked() override;
        virtual int GetHoverButtonCount(int row) override;
        virtual int GetHoverButtonColumn() override;
        virtual IconMgr::IconType GetHoverButtonIcon(int index, int row) override;
        virtual std::wstring GetHoverButtonTooltip(int index, int row) override;
        virtual void OnHoverButtonClicked(int btn_index, int row) override;
        virtual int GetUnHoverIconCount(int row) override;
        virtual IconMgr::IconType GetUnHoverIcon(int index, int row) override;
        virtual void DrawHoverButton(int index, int row);
        virtual void DrawUnHoverButton(CRect rc_button, int index, int row);

        virtual bool IsMultipleSelectionEnable() override { return true; }
        virtual void OnRowCountChanged() override;

        virtual bool IsItemMatchKeyWord(int row, const std::wstring& key_word);

    private:
        bool HasMultiVersion(int row) const;

        //��ȡ��ť������
        /*
          ��ť���У�
                    |   �޶���汾         |  �ж���汾
          ���ָ��   |  ���ţ���ӣ���ϲ��   |  ���ţ���ӣ���ϲ��������汾
          ��겻ָ�� |  ��ϲ��            |  ��ϲ��������汾
        */
        int PlayBtnIndex(int row, bool hover) const;
        int AddBtnIndex(int row, bool hover) const;
        int FavouriteBtnIndex(int row, bool hover) const;
        int SongVersionBtnIndex(int row, bool hover) const;

    private:
        int last_highlight_row{ -1 };
    };
}
