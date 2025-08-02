#pragma once
#include "ListElement.h"
namespace UiElement
{
    //���ؼ�
    //������ֻ��Ҫ�̳�GetRootNodes���������������ݼ���
    class TreeElement : public ListElement
    {
    public:
        //����һ���ڵ�
        struct Node
        {
            std::map<int, std::wstring> texts;   //һ�е��ı���key���кţ�value���ı�
            std::vector<std::shared_ptr<Node>> child_list;     //�ӽڵ��б�
            Node* parent{};     //���ڵ�
            bool collapsed{};   //�Ƿ��۵�

            void AddChild(std::shared_ptr<Node> child);
            int GetLevel() const;       //��ȡ�ڵ�ļ�������ڵ�û�и��ڵ㣬�򼶱�Ϊ0

            //��˳������ӽڵ�
            //func�������ڵ�ʱ�Ļص����������Ҫ�����������򷵻�true�����򷵻�false
            //ignore_invisible�����Ա��۵��Ľڵ�
            void IterateNodeInOrder(std::function<bool(Node*)> func, bool ignore_invisible);
        };

        virtual std::vector<std::shared_ptr<Node>>& GetRootNodes() = 0;   //��ȡ�����ڵ�

        int GetItemLevel(int row);          //��ȡ���еļ��𣨼���ÿ��1����һ�л�����һ�����룩
        bool IsCollapsable(int row);        //�����Ƿ�����۵������Ϊtrue������ʾ�۵�ͼ�꣩
        bool IsCollapsed(int row);          //�����Ƿ��۵�

        // ͨ�� Element �̳�
        virtual void LButtonUp(CPoint point) override;
        virtual void MouseMove(CPoint point) override;
        virtual void MouseLeave() override;
        virtual bool DoubleClick(CPoint point) override;

        // ͨ�� ListElement �̳�
        std::wstring GetItemText(int row, int col) override;
        int GetRowCount() override;
        //���ؼ���ʹ�û���ListElement�������߼�
        virtual void QuickSearch(const std::wstring& key_word) override;
        virtual void OnRowCountChanged() override;

        std::map<int, CRect> collapsd_rects;     //�۵���־�ľ�������key���У�
        int collaps_indicator_hover_row{ -1 };    //���ָ����۵���־���к�

    protected:
        int GetNodeIndex(const Node* node);     //����һ���ڵ����ţ�����ڵ㱻�۵��򲻴����򷵻�-1��
        Node* GetNodeByIndex(int index);    //����һ���ڵ����Ų��ҽڵ㣨���Ա��۵��Ľڵ㣩
        bool IsNodeMathcKeyWord(const Node* node, const std::wstring& key_word);  //�ж�һ���ڵ��Ƿ�ƥ��ؼ���
        bool IsNodeDisplayed(const Node* node);
        void IterateDisplayedNodeInOrder(std::function<bool(Node*)> func);      //�������пɼ��Ľڵ�
        std::set<const Node*> tree_search_result; //�����������
        bool tree_searched{};               //�Ƿ�������״̬
    };

    class TestTree : public TreeElement
    {
    public:
        TestTree();
        static std::shared_ptr<Node> CreateNode(std::wstring name, std::shared_ptr<Node> parent);

        virtual IconMgr::IconType GetIcon(int row) { return IconMgr::IT_Folder; }
        virtual bool HasIcon() { return true; }
        virtual int GetColumnCount() override;
        virtual int GetColumnWidth(int col, int total_width) override;
        virtual std::vector<std::shared_ptr<Node>>& GetRootNodes() override;

    private:
        std::vector<std::shared_ptr<Node>> root_nodes;
    };
}

