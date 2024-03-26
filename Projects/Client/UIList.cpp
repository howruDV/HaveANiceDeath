#include "pch.h"
#include "UIList.h"

UIList::UIList()
	: UI("", "##List")
    , m_CallBackFunc(nullptr)
    , m_Filter{}
{
	Deactivate();
    SetModal(true);
}

UIList::~UIList()
{
}

void UIList::render_update()
{
    ImVec2 vWinSize = ImGui::GetWindowContentRegionMax();
    vWinSize.y -= 27;
       
    static char buffer[32];
    strcpy_s(buffer, m_Filter.c_str());
    ImGui::Text("Filter"); ImGui::SameLine(80); ImGui::InputText("##ListFilter", buffer, 32);
    m_Filter = buffer;

    static int item_current_idx = 0; // Here we store our selection data as an index.
    if (ImGui::BeginListBox("##ListBox", vWinSize))
    {
        for (int i = 0; i < m_vecStr.size(); i++)
        {
            if (m_vecStr[i].find(m_Filter) == std::string::npos)
            {
                continue;
            }

            const bool is_selected = (item_current_idx == i);

            if (ImGui::Selectable(m_vecStr[i].c_str(), is_selected))
                item_current_idx = i;

            // ����Ʈ �� �ش� �׸��� Ŭ���Ǹ� ���̶���Ʈ �ɾ���
            if (is_selected)
                ImGui::SetItemDefaultFocus();

            // ����Ʈ �׸� �� ����Ŭ���� �߻��Ѵٸ�            
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                m_strDBClicked = m_vecStr[i];

                // ��ϵ� CallBack �� ������ ȣ��
                if (nullptr != m_CallBackFunc)
                {
                    m_CallBackFunc((DWORD_PTR)m_strDBClicked.c_str());
                }

                // ��ϵ� Delegate �� ������ ȣ��
                if (m_pUI && m_Func)
                {
                    (m_pUI->*m_Func)((DWORD_PTR)m_strDBClicked.c_str());
                }                 

                Deactivate();
            }
        }
        ImGui::EndListBox();
    }
}

void UIList::Deactivate()
{
	UI::Deactivate();
	m_vecStr.clear();
    ImGui::SetWindowFocus(nullptr);
}