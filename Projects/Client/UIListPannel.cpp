#include "pch.h"
#include "UIListPannel.h"

UIListPannel::UIListPannel()
	: UI("", "##List")
	, m_CallbackFunc(nullptr)
	, m_DelegateFunc(nullptr)
	, m_pUI(nullptr)
{
	Deactivate();
}

UIListPannel::~UIListPannel()
{
}

void UIListPannel::render_update()
{
	ImVec2 vWinSize = ImGui::GetContentRegionAvail();

	static int item_current_idx = 0;
	if (ImGui::BeginListBox(GetID().c_str(), vWinSize))
	{
		for (int i = 0; i < m_vecStr.size(); ++i)
		{
			const bool is_selected = (item_current_idx == i);

			// render
			if (is_selected)
				ImGui::SetItemDefaultFocus();

			// select
			if (ImGui::Selectable(m_vecStr[i].c_str(), is_selected))
				item_current_idx = i;
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				m_strDBClicked = m_vecStr[i];
				if (m_CallbackFunc)
					m_CallbackFunc((DWORD_PTR)m_strDBClicked.c_str());
				if (m_pUI && m_DelegateFunc)
					(m_pUI->*m_DelegateFunc)((DWORD_PTR)m_strDBClicked.c_str());
				
				Deactivate();
			}
		}

		ImGui::EndListBox();
	}
}

void UIListPannel::Deactivate()
{
	UI::Deactivate();
	m_vecStr.clear();	// 창 띄울 때마다 vec새로 불러올 것
	ImGui::SetWindowFocus(nullptr);
}
