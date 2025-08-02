#include "stdafx.h"
#include "Layout.h"


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void UiElement::Layout::CalculateChildrenRect()
{
    //ˮƽ����
    if (type == Horizontal)
    {
        vector<int> size_list;          // ��ȷ����Ԫ�سߴ��¼(�����߾�)��δȷ����ΪINT_MIN
        int total_size{};               // ������ָ��Ԫ�صĿ��(�Ǹ������)����ȷ�����ܱ߾�
        int item_fixed_size_num{};      // �й̶���ȵ�Ԫ�صĸ���

        // ��һ�α�������ȡ�̶�����ĳߴ�����
        for (const auto& child : childLst)
        {
            if (!child->IsEnable(GetRect()))            // ����Ϊ����ʾʱ���ߴ�Ϊ0�Ĺ̶��ߴ�Ԫ�ش��������Դ�Ԫ�ر߾�
            {
                size_list.push_back(0);
                item_fixed_size_num++;
            }
            else
            {
                if (child->IsWidthValid() && child->proportion < 1)    // proportion�趨ʱ����width
                {
                    int width{ child->GetWidth(GetRect()) };
                    total_size += width;
                    size_list.push_back(width);
                    item_fixed_size_num++;
                }
                else
                {
                    size_list.push_back(INT_MIN); // �����Ԫ�سߴ�δ��
                }
                if (child->margin_left.IsValid())
                    total_size += child->margin_left.GetValue(GetRect());
                if (child->margin_right.IsValid())
                    total_size += child->margin_right.GetValue(GetRect());
            }
        }

        int left_space{};                // ȫ�����й̶��ߴ�ʱ����Ԫ���뿪ʼ��Ե�ļ��
        bool all_ok{};
        while (!all_ok)
        {
            //���ÿ��Ԫ�ض��й̶��ĳߴ磬������ЩԪ���ڲ����о���
            if (childLst.size() == item_fixed_size_num)
            {
                left_space = (GetRect().Width() - total_size) / 2;
                if (left_space < 0)      // �ռ䲻��ʱ������ʾ����ǰ��Ԫ��
                    left_space = 0;
                all_ok = true;
            }
            else
            {
                // ��ʱsize_list��ΪINT_MIN����Ԫ��Ӧ����������
                int proportion{};                           // ��δ�̶���Ԫ�ر���ϵ����
                for (size_t i{}; i < childLst.size(); ++i)  // �������ϵ����
                {
                    if (size_list[i] == INT_MIN)
                        proportion += max(childLst[i]->proportion, 1);  // ��δ����ʱ��1����
                }
                // �������Ƿ������ֵ
                bool ok{ true };
                for (size_t i{}; i < childLst.size(); ++i)
                {
                    if (size_list[i] == INT_MIN)
                    {
                        auto& child{ childLst[i] };
                        int size{ (GetRect().Width() - total_size) * max(child->proportion, 1) / proportion };
                        int max_size{ child->GetMaxWidth(GetRect()) };
                        int min_size{ child->min_width.IsValid() ? child->min_width.GetValue(GetRect()) : 0 };
                        if (size < min_size || max_size < min_size)    // ��������ֵ��ͻʱ����ֵ��������Ԫ�ر��Ϊ�̶��ߴ�Ԫ�أ������ı��������������ֵ���ܱ�Ԥ��С�ʸ�����Сֵ���ߵ����ȼ�
                        {
                            size_list[i] = min_size;
                            total_size += min_size;
                            item_fixed_size_num++;
                            ok = false;
                            break;
                        }
                        else if (size > max_size)
                        {
                            size_list[i] = max_size;
                            total_size += max_size;
                            item_fixed_size_num++;
                            ok = false;
                            break;
                        }
                    }
                }
                if (!ok)        // okΪfalse˵��������һ���̶�Ԫ�أ����¼������
                {
                    continue;
                }
                else            // okΪtrue˵����ǰ������������ֵҪ��������ʽ���б���Ӧ��
                {
                    for (size_t i{}; i < childLst.size(); ++i)
                    {
                        if (size_list[i] == INT_MIN)
                        {
                            auto& child{ childLst[i] };
                            int size{ (GetRect().Width() - total_size) * max(child->proportion, 1) / proportion };
                            size_list[i] = max(size, 0);
                        }
                    }
                    left_space = 0;
                    all_ok = true;
                }
            }
        }
        ASSERT(find(size_list.begin(), size_list.end(), INT_MIN) == size_list.end());

        //����ÿ����Ԫ�صľ�������
        int w{};
        bool first_child{ true };
        for (size_t i{}; i < childLst.size(); i++)
        {
            auto& child{ childLst[i] };
            CRect child_rect{};
            if (child->IsHeightValid())
            {
                int child_height = child->GetHeight(GetRect());
                int max_height = GetRect().Height() - child->margin_top.GetValue(GetRect()) - child->margin_bottom.GetValue(GetRect());
                if (child_height > max_height)
                    child_height = max_height;
                child_rect.top = GetRect().top + (GetRect().Height() - child_height) / 2;
                child_rect.bottom = child_rect.top + child->GetHeight(GetRect());
            }
            else
            {
                child_rect.top = GetRect().top + child->margin_top.GetValue(GetRect());
                child_rect.bottom = GetRect().bottom - child->margin_bottom.GetValue(GetRect());
            }
            if (child->IsEnable(GetRect()))
            {
                if (first_child)
                {
                    child_rect.left = GetRect().left + child->margin_left.GetValue(GetRect()) + left_space;
                    first_child = false;
                }
                else
                {
                    child_rect.left = w + child->margin_left.GetValue(GetRect());
                }
                child_rect.right = child_rect.left + size_list[i];
                w = child_rect.right + child->margin_right.GetValue(GetRect());
            }
            else
            {
                child_rect.left = w;
                child_rect.right = w;
            }
            child->SetRect(child_rect);
        }
    }
    //��ֱ����
    else
    {
        vector<int> size_list;          // ��ȷ����Ԫ�سߴ��¼(�����߾�)��δȷ����ΪINT_MIN
        int total_size{};               // ������ָ��Ԫ�صĸ߶�(�Ǹ����߶�)����ȷ�����ܱ߾�
        int item_fixed_size_num{};      // �й̶��߶ȵ�Ԫ�صĸ���

        // ��һ�α�������ȡ�̶�����ĳߴ�����
        for (const auto& child : childLst)
        {
            if (!child->IsEnable(GetRect()))            // ����Ϊ����ʾʱ���ߴ�Ϊ0�Ĺ̶��ߴ�Ԫ�ش���
            {
                size_list.push_back(0);
                item_fixed_size_num++;
            }
            else
            {
                if (child->IsHeightValid() && child->proportion < 1)       // proportion�趨ʱ����height
                {
                    int height{ child->GetHeight(GetRect()) };
                    total_size += height;
                    size_list.push_back(height);
                    item_fixed_size_num++;
                }
                else
                {
                    size_list.push_back(INT_MIN); // �����Ԫ�سߴ�δ��
                }
                if (child->margin_top.IsValid())
                    total_size += child->margin_top.GetValue(GetRect());
                if (child->margin_bottom.IsValid())
                    total_size += child->margin_bottom.GetValue(GetRect());
            }
        }

        int top_space{};                // ȫ�����й̶��ߴ�ʱ����Ԫ���뿪ʼ��Ե�ļ��
        bool all_ok{};
        while (!all_ok)
        {
            //���ÿ��Ԫ�ض��й̶��ĳߴ磬������ЩԪ���ڲ����о���
            if (childLst.size() == item_fixed_size_num)
            {
                top_space = (GetRect().Height() - total_size) / 2;
                if (top_space < 0)      // �ռ䲻��ʱ������ʾ����ǰ��Ԫ��
                    top_space = 0;
                all_ok = true;
            }
            else
            {
                // ��ʱsize_list��ΪINT_MIN����Ԫ��Ӧ����������
                int proportion{};                           // ��δ�̶���Ԫ�ر���ϵ����
                for (size_t i{}; i < childLst.size(); ++i)  // �������ϵ����
                {
                    if (size_list[i] == INT_MIN)
                        proportion += max(childLst[i]->proportion, 1);  // ��δ����ʱ��1����
                }
                // �������Ƿ������ֵ
                bool ok{ true };
                for (size_t i{}; i < childLst.size(); ++i)
                {
                    if (size_list[i] == INT_MIN)
                    {
                        auto& child{ childLst[i] };
                        int size{ (GetRect().Height() - total_size) * max(child->proportion, 1) / proportion };
                        int max_size{ child->max_height.IsValid() ? child->max_height.GetValue(GetRect()) : INT_MAX };
                        int min_size{ child->min_height.IsValid() ? child->min_height.GetValue(GetRect()) : 0 };
                        if (size < min_size || max_size < min_size)                // ��������ֵ��ͻʱ����ֵ��������Ԫ�ر��Ϊ�̶��ߴ�Ԫ��
                        {
                            size_list[i] = min_size;
                            total_size += min_size;
                            item_fixed_size_num++;
                            ok = false;
                            break;
                        }
                        else if (size > max_size)
                        {
                            size_list[i] = max_size;
                            total_size += max_size;
                            item_fixed_size_num++;
                            ok = false;
                            break;
                        }
                    }
                }
                if (!ok)        // okΪfalse˵��������һ���̶�Ԫ�أ����¼������
                {
                    continue;
                }
                else            // okΪtrue˵����ǰ������������ֵҪ��������ʽ���б���Ӧ��
                {
                    for (size_t i{}; i < childLst.size(); ++i)
                    {
                        if (size_list[i] == INT_MIN)
                        {
                            auto& child{ childLst[i] };
                            int size{ (GetRect().Height() - total_size) * max(child->proportion, 1) / proportion };
                            size_list[i] = max(size, 0);
                        }
                    }
                    top_space = 0;
                    all_ok = true;
                }
            }
        }
        ASSERT(find(size_list.begin(), size_list.end(), INT_MIN) == size_list.end());

        // ����ÿ����Ԫ�صľ�������
        int h{};
        bool first_child{ true };
        for (size_t i{}; i < childLst.size(); i++)
        {
            auto& child{ childLst[i] };
            CRect child_rect{};
            if (child->IsWidthValid())
            {
                int child_width = child->GetWidth(GetRect());
                int max_width = GetRect().Width() - child->margin_left.GetValue(GetRect()) - child->margin_right.GetValue(GetRect());
                if (child_width > max_width)
                    child_width = max_width;
                child_rect.left = GetRect().left + (GetRect().Width() - child_width) / 2;
                child_rect.right = child_rect.left + child->GetWidth(GetRect());
            }
            else
            {
                child_rect.left = GetRect().left + child->margin_left.GetValue(GetRect());
                child_rect.right = GetRect().right - child->margin_right.GetValue(GetRect());
            }
            if (child->IsEnable(GetRect()))
            {
                if (first_child)
                {
                    child_rect.top = GetRect().top + child->margin_top.GetValue(GetRect()) + top_space;
                    first_child = false;
                }
                else
                {
                    child_rect.top = h + child->margin_top.GetValue(GetRect());
                }
                child_rect.bottom = child_rect.top + size_list[i];
                h = child_rect.bottom + child->margin_bottom.GetValue(GetRect());
            }
            else
            {
                child_rect.top = h;
                child_rect.bottom = h;
            }
            child->SetRect(child_rect);
        }
    }
}


void UiElement::Layout::Draw()
{
    CalculateRect();
    CalculateChildrenRect();
    Element::Draw();
}
