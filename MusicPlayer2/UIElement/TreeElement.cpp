#include "stdafx.h"
#include "TreeElement.h"
#include <stack>

void UiElement::TreeElement::Node::AddChild(std::shared_ptr<Node> child)
{
    child->parent = this;
    child_list.push_back(child);
}

int UiElement::TreeElement::Node::GetLevel() const
{
    int level{};
    const Node* node{ this };
    while (node != nullptr && node->parent != nullptr)
    {
        node = node->parent;
        level++;
    }
    return level;
}

void UiElement::TreeElement::Node::IterateNodeInOrder(std::function<bool(Node*)> func, bool ignore_invisible)
{
    std::stack<UiElement::TreeElement::Node*> nodeStack;
    nodeStack.push(this);
    while (!nodeStack.empty())
    {
        UiElement::TreeElement::Node* pCurNode = nodeStack.top();
        nodeStack.pop();

        if (func(pCurNode))
            break;

        //�����ǰ�ڵ��Ѿ��۵�������Ҫ�������۵��Ľڵ㣬���ٱ������ӽڵ�
        if (pCurNode->collapsed && ignore_invisible)
            continue;

        for (auto& child : pCurNode->child_list)
        {
            nodeStack.push(child.get());
        }
    }
}

std::wstring UiElement::TreeElement::GetItemText(int row, int col)
{
    //���ҽڵ�
    const Node* node = GetNodeByIndex(row);
    if (node != nullptr)
    {
        auto iter = node->texts.find(col);
        if (iter != node->texts.end())
            return iter->second;
    }
    return std::wstring();
}

int UiElement::TreeElement::GetRowCount()
{
    int row_count{};
    IterateDisplayedNodeInOrder([&](const Node*) ->bool {
        row_count++;
        return false;
        });
    return row_count;
}

void UiElement::TreeElement::QuickSearch(const std::wstring& key_word)
{
    tree_searched = !key_word.empty();

    tree_search_result.clear();
    if (key_word.empty())
        return;
    //�������нڵ㣬��ȡƥ��Ľڵ㣬����ӵ�tree_search_result��
    auto& root_nodes{ GetRootNodes() };
    for (auto& root : root_nodes)
    {
        root->IterateNodeInOrder([&](Node* cur_node) ->bool {
            if (IsNodeMathcKeyWord(cur_node, key_word))
            {
                tree_search_result.insert(cur_node);
                cur_node->collapsed = false;    //ƥ��Ľڵ�ȫ��չ��
            }
            return false;
            }, false);
    }
}

void UiElement::TreeElement::OnRowCountChanged()
{
    //���ؼ��������仯����ֻ�ǽڵ��չ�����۵�����˲�ִ�л�����OnRowCountChanged�Ĵ���
}

int UiElement::TreeElement::GetItemLevel(int row)
{
    const Node* node = GetNodeByIndex(row);
    if (node != nullptr)
        return node->GetLevel();
    return 0;
}

bool UiElement::TreeElement::IsCollapsable(int row)
{
    const Node* node = GetNodeByIndex(row);
    if (node != nullptr)
        return !node->child_list.empty();
    return false;
}

bool UiElement::TreeElement::IsCollapsed(int row)
{
    const Node* node = GetNodeByIndex(row);
    if (node != nullptr)
        return node->collapsed;
    return false;
}

void UiElement::TreeElement::LButtonUp(CPoint point)
{
    //��ȡ�������
    int row = GetListIndexByPoint(point);
    if (row >= 0)
    {
        auto iter = collapsd_rects.find(row);
        if (iter != collapsd_rects.end())
        {
            CRect rect_collapsd = iter->second;
            //������۵���־
            if (rect_collapsd.PtInRect(point))
            {
                Node* node = GetNodeByIndex(row);
                node->collapsed = !node->collapsed;
            }
        }
    }

    ListElement::LButtonUp(point);
}

void UiElement::TreeElement::MouseMove(CPoint point)
{
    //��ȡ���ָ�����
    int row = GetListIndexByPoint(point);
    collaps_indicator_hover_row = -1;
    if (row >= 0)
    {
        auto iter = collapsd_rects.find(row);
        if (iter != collapsd_rects.end())
        {
            CRect rect_collapsd = iter->second;
            //ָ�����۵���־
            if (rect_collapsd.PtInRect(point))
            {
                collaps_indicator_hover_row = row;
            }
        }
    }

    ListElement::MouseMove(point);
}

void UiElement::TreeElement::MouseLeave()
{
    collaps_indicator_hover_row = -1;
    ListElement::MouseLeave();
}

bool UiElement::TreeElement::DoubleClick(CPoint point)
{
    //���˫�����۵���־����ִ��˫������
    for (const auto& rect : collapsd_rects)
    {
        if (rect.second.PtInRect(point))
            return false;
    }
    return ListElement::DoubleClick(point);
}

int UiElement::TreeElement::GetNodeIndex(const Node* node)
{
    int i{};
    int rtn_index{ -1 };
    IterateDisplayedNodeInOrder([&](const Node* cur_node) ->bool {
        if (cur_node == node)
        {
            rtn_index = i;
            return true;
        }
        i++;
        return false;
        });

    return rtn_index;
}

UiElement::TreeElement::Node* UiElement::TreeElement::GetNodeByIndex(int index)
{
    if (index >= 0)
    {
        Node* find_node{};
        int i{};
        IterateDisplayedNodeInOrder([&](Node* cur_node) ->bool {
            if (i == index)
            {
                find_node = cur_node;
                return true;
            }
            i++;
            return false;
            });
        return find_node;
    }

    return nullptr;
}

bool UiElement::TreeElement::IsNodeMathcKeyWord(const Node* node, const std::wstring& key_word)
{
    //�жϽڵ㱾���Ƿ�ƥ��
    for (const auto& item : node->texts)
    {
        const std::wstring& text{ item.second };
        if (!text.empty() && theApp.m_chinese_pingyin_res.IsStringMatchWithPingyin(key_word, text))
            return true;
    }

    //����ڵ㱾��ƥ�䣬����������ӽڵ㣬�����һ���ӽڵ�ƥ�䣬��ڵ�ƥ��
    for (const auto& child : node->child_list)
    {
        if (IsNodeMathcKeyWord(child.get(), key_word))
            return true;
    }

    return false;
}

bool UiElement::TreeElement::IsNodeDisplayed(const Node* node)
{
    if (node != nullptr)
    {
        if (tree_searched)
            return tree_search_result.contains(node);
        else
            return true;
    }
    return false;
}

void UiElement::TreeElement::IterateDisplayedNodeInOrder(std::function<bool(Node*)> func)
{
    const auto& root_nodes{ GetRootNodes() };
    for (const auto& root : root_nodes)
    {
        bool exit{};
        root->IterateNodeInOrder([&](Node* cur_node) ->bool {
            if (IsNodeDisplayed(cur_node))
            {
                if (func(cur_node))
                {
                    exit = true;
                    return true;
                }
            }
            return false;
            }, true);
        if (exit)
            break;
    }
}

UiElement::TestTree::TestTree()
{
    //�������Խڵ�
    std::shared_ptr<Node> root1 = CreateNode(L"���ڵ�1", nullptr);
    std::shared_ptr<Node> root2 = CreateNode(L"���ڵ�2", nullptr);

    CreateNode(L"�ӽڵ�11", root1);
    auto node12 = CreateNode(L"�ӽڵ�12", root1);

    CreateNode(L"�ӽڵ�121", node12);
    CreateNode(L"�ӽڵ�122", node12);

    CreateNode(L"�ӽڵ�21", root2);
    CreateNode(L"�ӽڵ�22", root2);

    root_nodes.push_back(root1);
    root_nodes.push_back(root2);
}

std::shared_ptr<UiElement::TreeElement::Node> UiElement::TestTree::CreateNode(std::wstring name, std::shared_ptr<Node> parent)
{
    std::shared_ptr<Node> node = std::make_shared<Node>();
    node->collapsed = true;
    node->texts[0] = name;
    if (parent != nullptr)
        parent->AddChild(node);
    return node;
}

int UiElement::TestTree::GetColumnCount()
{
    return 1;
}

int UiElement::TestTree::GetColumnWidth(int col, int total_width)
{
    return total_width;
}

std::vector<std::shared_ptr<UiElement::TestTree::Node>>& UiElement::TestTree::GetRootNodes()
{
    return root_nodes;
}