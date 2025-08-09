#pragma once
#include "TreeElement.h"
namespace UiElement
{
    //ý����е��ļ������
    class FolderExploreTree : public TreeElement
    {
    public:
        enum Column
        {
            COL_NAME,
            COL_COUNT,
            COL_MAX
        };

        //���ָ��һ��ʱ��ʾ�İ�ť
        enum BtnKey
        {
            BTN_PLAY,
            BTN_ADD,
            BTN_MAX
        };

        static std::shared_ptr<Node> CreateNode(std::wstring name, int song_num, std::shared_ptr<Node> parent);
        static std::wstring GetNodePath(Node* node);       //��ȡһ���ڵ��·��
        std::wstring GetSelectedPath();             //��ȡѡ�нڵ��·��

        virtual IconMgr::IconType GetIcon(int row) { return IconMgr::IT_Folder; }
        virtual bool HasIcon() { return true; }
        virtual int GetColumnCount() override;
        virtual int GetColumnWidth(int col, int total_width) override;
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

        virtual std::vector<std::shared_ptr<Node>>& GetRootNodes() override;
    };

}

