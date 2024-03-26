#include "pch.h"
#include "UICamera.h"
#include "UIListPannel.h"
#include "func_ImGUI.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CCamera.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

UICamera::UICamera()
	: UIComponent("Camera##", "Camera", COMPONENT_TYPE::CAMERA)
{
}

UICamera::~UICamera()
{
}

void UICamera::render_update()
{
	if (not GetTargetObject())
		return;

	UIComponent::render_update();

	CCamera* pCamera = GetTargetObject()->Camera();
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	// Get layer name
	for (int i = 0; i < LAYER_MAX; ++i)
	{
		wstring layerName = pLevel->GetLayer(i)->GetName();
		string strLayerName = string(layerName.begin(), layerName.end());
		if (strLayerName == "")
		{
			strLayerName = std::to_string(i);
		}

		m_vecLayerName.push_back("[" + std::to_string(i) + "]" + " " + strLayerName);
	}

	// Priority
	TextBox("Camera Priority"); ImGui::SameLine();
	int Priority = pCamera->GetPriority();
	ImGui::InputInt("##Priority", &Priority);

	if (Priority >= 0)
	{
		pCamera->SetCameraPriority(Priority);
	}

	// Projection Type 설정 드롭박스
	PROJ_TYPE CurType = pCamera->GetProjType();
	string ProjType = "";

	switch (CurType)
	{
	case PROJ_TYPE::ORTHOGRAPHIC:
		ProjType = "ORTHOGRAPHIC";
		break;
	case PROJ_TYPE::PERSPECTIVE:
		ProjType = "PERSPECTIVE";
		break;
	default:
		break;
	}

	TextBox("Projection Type"); ImGui::SameLine();

	if (ImGui::BeginCombo("##ProJecTypeCombo", ProjType.data())) {
		if (ImGui::Selectable("ORTHOGRAPHIC"))
		{
			pCamera->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		}
		if (ImGui::Selectable("PERSPECTIVE"))
		{
			pCamera->SetProjType(PROJ_TYPE::PERSPECTIVE);
		}

		ImGui::EndCombo();
	}

	if (CurType == PROJ_TYPE::PERSPECTIVE)
	{
		// FOV
		TextBox("FOV"); ImGui::SameLine();
		float FOV = pCamera->GetFOV();
		ImGui::DragFloat("##CameraFOV", &FOV, 0.1, 0.1f, 3.14f);
		pCamera->SetFOV(FOV);
	}
	else
	{
		// Scale
		TextBox("Scale"); ImGui::SameLine();
		float Scale = pCamera->GetScale();
		ImGui::DragFloat("##CameraScale", &Scale);
		if (Scale != 0.f)
		{
			pCamera->SetScale(Scale);
		}
	}

	// Far
	TextBox("Far"); ImGui::SameLine();
	float Far = pCamera->GetFar();
	ImGui::DragFloat("##CameraFar", &Far);
	if (Far > 1.f)
	{
		pCamera->SetFar(Far);
	}

	// Layer Check
	vector<string> items;
	vector<string> NoneCheckLayer;

	for (int i = 0; i < LAYER_MAX; ++i)
	{
		if (pCamera->IsLayerCheck(i))
		{
			items.push_back(m_vecLayerName[i]);
		}
		else
		{
			NoneCheckLayer.push_back(m_vecLayerName[i]);
		}
	}

	TextBox("Layer Check");
	ImGui::SameLine();

	ImGui::BeginGroup();
	{
		static int item_current_idx = 0;
		if (ImGui::BeginListBox("##CheckLayerList", ImVec2(ImGui::CalcItemWidth(), 5 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (int i = 0; i < items.size(); i++)
			{
				const bool is_selected = (item_current_idx == i);

				if (ImGui::Selectable(items[i].c_str(), is_selected))
					item_current_idx = i;

				// 리스트 중 해당 항목이 클릭되면 하이라이트 걸어줌
				if (is_selected)
					ImGui::SetItemDefaultFocus();

			}
			ImGui::EndListBox();
		}

		ImVec2 buttonSize{ 155.f, 25.f };
		float posX = (ImGui::GetCursorPos().x + ImGui::CalcItemWidth()) - buttonSize.x * 2 - ImGui::GetStyle().ItemSpacing.x;
		ImGui::SetCursorPosX(posX);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 0)); // 버튼 간 간격 조정

		if (ImGui::Button("Check", buttonSize))
		{
			UIListPannel* pListUI = (UIListPannel*)CImGuiMgr::GetInst()->FindUI("##List");

			pListUI->AddString(NoneCheckLayer);
			pListUI->SetDBClickeDelegate(this, (DELEGATE_1)&UICamera::CheckLayer);
			pListUI->Activate();
		}

		ImGui::SameLine(0, ImGui::GetStyle().ItemSpacing.x);

		if (ImGui::Button("UnCheck", buttonSize))
		{
			if (item_current_idx < items.size())
			{
				string TargetLayer = items[item_current_idx];
				int LayerIdx = -1;

				size_t start_pos = TargetLayer.find("[") + 1;
				size_t end_pos = TargetLayer.find("]");
				if (start_pos != std::string::npos && end_pos != std::string::npos) {
					std::string num_str = TargetLayer.substr(start_pos, end_pos - start_pos);
					LayerIdx = std::stoi(num_str);
				}

				pCamera->LayerCheck(LayerIdx, false);
			}
		}

		ImGui::PopStyleVar();
		ImGui::EndGroup();
	}

	m_vecLayerName.clear();
}

void UICamera::CheckLayer(DWORD_PTR _ptr)
{
	if (nullptr == GetTargetObject() || nullptr == GetTargetObject()->Camera())
		return;

	string strState = (char*)_ptr;
	int LayerIdx = -1;

	size_t start_pos = strState.find("[") + 1;
	size_t end_pos = strState.find("]");
	if (start_pos != std::string::npos && end_pos != std::string::npos) {
		std::string num_str = strState.substr(start_pos, end_pos - start_pos);
		LayerIdx = std::stoi(num_str);
	}

	GetTargetObject()->Camera()->LayerCheck(LayerIdx, true);
}