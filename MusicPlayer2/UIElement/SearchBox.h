#pragma once
#include "UIElement/UIElement.h"
class CUiSearchBox;
namespace UiElement
{
    //������
    class SearchBox : public Element
    {
    public:
        SearchBox();
        ~SearchBox();
        void InitSearchBoxControl(CWnd* pWnd);  //��ʼ��������ؼ���pWnd��������
        void OnKeyWordsChanged();
        void Clear();
        ListElement* GetListElement() { return list_element; }

        virtual void Draw() override;
        virtual void MouseMove(CPoint point) override;
        virtual void MouseLeave() override;
        virtual void LButtonUp(CPoint point) override;
        virtual void LButtonDown(CPoint point) override;

        bool hover{};       //������ָ����������Ϊtrue
        std::wstring key_word;  //�������е��ı�
        CUiSearchBox* search_box_ctrl{};    //������ؼ�
        CRect icon_rect;    //ͼ�������
        CPlayerUIBase::UIButton clear_btn;      //�����ť

    private:
        void FindListElement();         //����ListElement
        bool find_list_element{};       //����Ѿ����ҹ�ListElement����Ϊtrue
        ListElement* list_element{};    //������ListElement
    };
}

