#pragma once
#include "UIElement/UIElement.h"
namespace UiElement
{
    //�б�Ԫ��
    class ListElement : public Element
    {
    public:
        friend class CPlayerUIBase;

        virtual void Draw() override;
        virtual void LButtonUp(CPoint point) override;
        virtual void LButtonDown(CPoint point) override;
        virtual void MouseMove(CPoint point) override;
        virtual bool RButtunUp(CPoint point) override;
        void ShowContextMenu(CMenu* menu, CWnd* cmd_reciver);
        virtual void RButtonDown(CPoint point) override;
        virtual bool MouseWheel(int delta, CPoint point) override;
        virtual void MouseLeave() override;
        virtual bool DoubleClick(CPoint point) override;
        virtual void ClearRect() override;

        void EnsureItemVisible(int index);  //ȷ��ָ�����ڲ����б��пɼ�
        void EnsureHighlightItemVisible();  //ȷ�������пɼ�
        void RestrictOffset();             //�������б�ƫ������������ȷ�ķ�Χ
        void CalculateItemRects();         //���㲥���б���ÿһ��ľ������򣬱�����playlist_info.item_rects��
        int ItemHeight() const;
        void SetItemSelected(int index);    //���õ�����Ŀѡ��
        int GetItemSelected() const;        //��ȡ������Ŀѡ��
        void SetItemsSelected(const vector<int>& indexes);  //���ö����Ŀѡ��
        void GetItemsSelected(vector<int>& indexes) const;  //��ȡ�����Ŀѡ��
        bool IsItemSelected(int index) const;   //�ж�ָ�����Ƿ�ѡ��
        bool IsMultipleSelected() const;        //�Ƿ�ѡ���˳���1����Ŀ

        void SelectAll();                   //ȫѡ����IsMultipleSelectionEnable����trueʱ֧�֣�
        void SelectNone();                  //ȡ������ѡ��
        void SelectReversed();              //����ѡ�񣨽�IsMultipleSelectionEnable����trueʱ֧�֣�

        virtual std::wstring GetItemText(int row, int col) = 0;
        virtual int GetRowCount() = 0;
        virtual int GetColumnCount() = 0;
        virtual int GetColumnWidth(int col, int total_width) = 0;
        virtual IconMgr::IconType GetIcon(int row) { return IconMgr::IT_NO_ICON; }
        virtual bool HasIcon() { return false; }
        virtual std::wstring GetEmptyString() { return std::wstring(); }    //�б�Ϊ��ʱ��ʾ���ı�
        virtual int GetHighlightRow() { return -1; }
        virtual int GetColumnScrollTextWhenSelected() { return -1; }    //��ȡѡ��ʱ��Ҫ������ʾ����
        virtual bool ShowTooltip() { return false; }
        virtual std::wstring GetToolTipText(int row) { return std::wstring(); }
        virtual int GetToolTipIndex() const { return 0; }
        virtual CMenu* GetContextMenu(bool item_selected) { return nullptr; }
        virtual CWnd* GetCmdRecivedWnd() { return nullptr; }        //��ȡ�Ҽ��˵�����Ľ��մ��ڣ�������ؿ�ָ�룬����CUIWindowCmdHelper����Ӧ
        virtual void OnDoubleClicked() {};
        virtual void OnClicked() {};
        virtual int GetHoverButtonCount() { return 0; }     //��ȡ���ָ��һ��ʱҪ��ʾ�İ�ť����
        virtual int GetHoverButtonColumn() { return 0; }    //��ȡ���ָ��ʱҪ��ʾ�İ�ť������
        virtual IconMgr::IconType GetHoverButtonIcon(int index, int row) { return IconMgr::IT_NO_ICON; } //��ȡ���ָ��һ��ʱ��ť��ͼ��
        virtual std::wstring GetHoverButtonTooltip(int index, int row) { return std::wstring(); }     //��ȡ���ָ��һ��ʱ��ť�������ʾ
        virtual void OnHoverButtonClicked(int btn_index, int row) {}    //��Ӧ���ָ��ʱ��ť���
        IPlayerUI::UIButton& GetHoverButtonState(int btn_index);        //��ȡ�������ָ��ʱ��ť��Ϣ�Ľṹ��
        virtual int GetUnHoverIconCount(int row) { return 0; }          //��ȡ���δָ�����Ҫ��ʾ��ͼ����������ΪGetHoverButtonColumn���ص��У�
        virtual IconMgr::IconType GetUnHoverIcon(int index, int row) { return IconMgr::IT_NO_ICON; }   //��ȡ���δָ�����Ҫ��ʾ��ͼ��

        virtual bool IsMultipleSelectionEnable() { return false; }      //�Ƿ������ѡ
        virtual void OnRowCountChanged();       //���б����������仯ʱ��Ӧ�˺���

        virtual void QuickSearch(const std::wstring& key_word);         //���ݹؼ�ִ�п���������ɸѡ��ƥ����
        virtual bool IsItemMatchKeyWord(int row, const std::wstring& key_word);     //�ж�ָ�����Ƿ�ƥ��ؼ��֣����ڿ����������ܣ�Ĭ��ƥ��ÿһ���е��ı���ֻҪ��һ�е��ı�ƥ��ͷ���true�����������д�˺�����

        int GetDisplayRowCount();       //��ȡҪ��ʾ������������������״̬ʱ���������������������״̬��ͬGetRowCount��
        bool IsRowDisplayed(int row);   //�ж�һ���Ƿ���ʾ��������������״̬ʱ��ƥ����л᷵��false��

        void SetRelatedSearchBox(SearchBox* search_box) { related_search_box = search_box; }

        int item_height{ 28 };
        int font_size{ 9 };

    private:
        void DisplayRowToAbsoluteRow(int& row); //����ʾ���к�ת��Ϊ�����к�
        void AbsoluteRowToDisplayRow(int& row); //�������к�ת��Ϊ��ʾ���к�
        int GetDisplayedIndexByPoint(CPoint point);

    protected:
        int GetListIndexByPoint(CPoint point);

    protected:
        bool mouse_pressed{ };          //�������Ƿ���
        bool hover{};                   //ָ��ָ�򲥷��б�����
        CPoint mouse_pos;               //���ָ�������
        CPoint mouse_pressed_pos;       //��갴��ʱ��λ��
        int mouse_pressed_offset{};     //��갴��ʱ�����б��λ��
        int playlist_offset{};          //��ǰ�����б������λ��
        std::set<int> items_selected; //ѡ�е����
        CDrawCommon::ScrollInfo selected_item_scroll_info;  //����ѡ��������ı��Ľṹ��
        std::vector<CRect> item_rects;  //�����б���ÿ����Ŀ�ľ�������
        CRect scrollbar_rect{};         //��������λ��
        CRect scrollbar_handle_rect;    //���������ֵ�λ��
        bool scrollbar_hover{};         //���ָ�������
        bool scrollbar_handle_pressed{};    //���������ֱ�����
        int scroll_handle_length_comp{};    //������������ֳ���ʱ�Ĳ�����
        std::map<int, IPlayerUI::UIButton> hover_buttons;   //���ָ��ʱ�İ�ť
        int last_row_count{};
    private:
        std::vector<int> search_result; //����������������
        bool searched{};                //�Ƿ�������״̬
        SearchBox* related_search_box{};    //�����ļ���
    };
}

