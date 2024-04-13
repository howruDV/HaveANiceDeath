#include "pch.h"
#include "MtrlEditorUI.h"
#include "UIParam.h"
#include "UIListPannel.h"

#include <Engine/CAssetMgr.h>

UIMatEditor::UIMatEditor()
	: UI("Mat Inspector", "##MatInspector")
	, m_IntArr{}
	, m_FloatArr{}
	, m_V2Arr{}
	, m_V4Arr{}
	, m_TargetMtrl(nullptr)
	, m_MtrlPath("New Material")
	, m_INT_Desc{ "INT_0_DESC","INT_1_DESC","INT_2_DESC","INT_3_DESC" }
	, m_FLOAT_Desc{ "FLOAT_0_DESC","FLOAT_1_DESC","FLOAT_2_DESC","FLOAT_3_DESC" }
	, m_V2_Desc{ "VEC2_0_DESC","VEC2_1_DESC","VEC2_2_DESC","VEC2_3_DESC" }
	, m_V4_Desc{ "VEC4_0_DESC","VEC4_1_DESC","VEC4_2_DESC","VEC4_3_DESC" }
	, m_TEX_Desc{ "TEX_0", "TEX_1", "TEX_2", "TEX_3", "TEX_4", "TEX_5" }
	, m_DescSize(200)
	, m_InputSize(100)
{
}

UIMatEditor::~UIMatEditor()
{
}


void UIMatEditor::render_update()
{
	// 버튼
	if (ImGui::Button("Save", ImVec2(80.f, 30.f)))
	{
		Save();
	}
	ImGui::SameLine();
	if (ImGui::Button("Load", ImVec2(80.f, 30.f)))
	{
		Load();
	}
	ImGui::SameLine();
	if (ImGui::Button("Create", ImVec2(100.f, 30.f)))
	{
		Create();
	}
	ImGui::Separator(); ImGui::Spacing(); ImGui::Spacing();


	if (nullptr == m_TargetMtrl)
		return;

	// 머터리얼 경로 
	ImGui::Text("Material"); ImGui::SameLine(100); ImGui::InputText("##MaterialPath", (char*)m_MtrlPath.data(), 64);

	// 쉐이더 이름
	ImGui::Text("Shader"); ImGui::SameLine(100); ImGui::InputText("##ShaderName", (char*)m_ShaderName.data(), 32, ImGuiInputTextFlags_ReadOnly);
	// Shader Drop 체크
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (payload)
		{
			DWORD_PTR data = *((DWORD_PTR*)payload->Data);
			CAsset* pAsset = (CAsset*)data;
			if (ASSET_TYPE::GRAPHICS_SHADER == pAsset->GetType())
			{
				m_TargetMtrl->SetShader((CGraphicsShader*)pAsset);
				m_ShaderName = WstrToStr(m_TargetMtrl->GetShader()->GetKey());
			}
		}

		ImGui::EndDragDropTarget();
	}


	ImGui::Separator(); ImGui::Spacing(); ImGui::Spacing();
	// ----------------------------------------------------------------------------------------------------------------

	ImGuiStyle& style = ImGui::GetStyle();
	style.FramePadding.x = 4;

	// =========
	// Parameter
	// =========

	// INT_PARAM
	if (ImGui::CollapsingHeader("INT_PARAM", ImGuiTreeNodeFlags_None))
	{
		ImGui::Text("INT_0"); ImGui::SameLine(100); ImGui::SetNextItemWidth(m_DescSize); ImGui::InputText("##INT_0_Desc", (char*)m_INT_Desc[0].data(), 32,ImGuiInputTextFlags_ReadOnly); ImGui::SameLine(m_DescSize + 120); ImGui::SetNextItemWidth(m_InputSize); ImGui::InputInt("##INT_0_DATA", &m_IntArr[0]);
		ImGui::Text("INT_1"); ImGui::SameLine(100); ImGui::SetNextItemWidth(m_DescSize); ImGui::InputText("##INT_1_Desc", (char*)m_INT_Desc[1].data(), 32,ImGuiInputTextFlags_ReadOnly); ImGui::SameLine(m_DescSize + 120); ImGui::SetNextItemWidth(m_InputSize); ImGui::InputInt("##INT_1_DATA", &m_IntArr[1]);
		ImGui::Text("INT_2"); ImGui::SameLine(100); ImGui::SetNextItemWidth(m_DescSize); ImGui::InputText("##INT_2_Desc", (char*)m_INT_Desc[2].data(), 32,ImGuiInputTextFlags_ReadOnly); ImGui::SameLine(m_DescSize + 120); ImGui::SetNextItemWidth(m_InputSize); ImGui::InputInt("##INT_2_DATA", &m_IntArr[2]);
		ImGui::Text("INT_3"); ImGui::SameLine(100); ImGui::SetNextItemWidth(m_DescSize); ImGui::InputText("##INT_3_Desc", (char*)m_INT_Desc[3].data(), 32,ImGuiInputTextFlags_ReadOnly); ImGui::SameLine(m_DescSize + 120); ImGui::SetNextItemWidth(m_InputSize); ImGui::InputInt("##INT_3_DATA", &m_IntArr[3]);

		ImGui::Separator(); ImGui::Spacing(); ImGui::Spacing();
	}


	// FLOAT_PARAM
	if (ImGui::CollapsingHeader("FLOAT_PARAM", ImGuiTreeNodeFlags_None))
	{
		ImGui::Text("FLOAT_0"); ImGui::SameLine(100); ImGui::SetNextItemWidth(m_DescSize); ImGui::InputText("##FLOAT_0_Desc", (char*)m_FLOAT_Desc[0].data(), 32,ImGuiInputTextFlags_ReadOnly); ImGui::SameLine(m_DescSize + 120); ImGui::SetNextItemWidth(m_InputSize); ImGui::InputFloat("##FLOAT_0_DATA", &m_FloatArr[0]);
		ImGui::Text("FLOAT_1"); ImGui::SameLine(100); ImGui::SetNextItemWidth(m_DescSize); ImGui::InputText("##FLOAT_1_Desc", (char*)m_FLOAT_Desc[1].data(), 32,ImGuiInputTextFlags_ReadOnly); ImGui::SameLine(m_DescSize + 120); ImGui::SetNextItemWidth(m_InputSize); ImGui::InputFloat("##FLOAT_1_DATA", &m_FloatArr[1]);
		ImGui::Text("FLOAT_2"); ImGui::SameLine(100); ImGui::SetNextItemWidth(m_DescSize); ImGui::InputText("##FLOAT_2_Desc", (char*)m_FLOAT_Desc[2].data(), 32,ImGuiInputTextFlags_ReadOnly); ImGui::SameLine(m_DescSize + 120); ImGui::SetNextItemWidth(m_InputSize); ImGui::InputFloat("##FLOAT_2_DATA", &m_FloatArr[2]);
		ImGui::Text("FLOAT_3"); ImGui::SameLine(100); ImGui::SetNextItemWidth(m_DescSize); ImGui::InputText("##FLOAT_3_Desc", (char*)m_FLOAT_Desc[3].data(), 32,ImGuiInputTextFlags_ReadOnly); ImGui::SameLine(m_DescSize + 120); ImGui::SetNextItemWidth(m_InputSize); ImGui::InputFloat("##FLOAT_3_DATA", &m_FloatArr[3]);

		ImGui::Separator(); ImGui::Spacing(); ImGui::Spacing();
	}

	// Vec2_Param
	if (ImGui::CollapsingHeader("VEC2_PARAM", ImGuiTreeNodeFlags_None))
	{
		ImGui::Text("VEC2_0"); ImGui::SameLine(100); ImGui::SetNextItemWidth(m_DescSize); ImGui::InputText("##VEC2_0_Desc", (char*)m_V2_Desc[0].data(), 32,ImGuiInputTextFlags_ReadOnly); ImGui::SameLine(); ImGui::SameLine(); ImGui::Text("  X"); ImGui::SameLine(); ImGui::SetNextItemWidth(60); ImGui::InputFloat("##VEC2_0_DATA_X", &m_V2Arr[0].x); ImGui::SameLine(); ImGui::Text(",  Y"); ImGui::SameLine(); ImGui::SetNextItemWidth(60); ImGui::InputFloat("##VEC2_0_DATA_Y", &m_V2Arr[0].y);
		ImGui::Text("VEC2_1"); ImGui::SameLine(100); ImGui::SetNextItemWidth(m_DescSize); ImGui::InputText("##VEC2_1_Desc", (char*)m_V2_Desc[1].data(), 32,ImGuiInputTextFlags_ReadOnly); ImGui::SameLine(); ImGui::SameLine(); ImGui::Text("  X"); ImGui::SameLine(); ImGui::SetNextItemWidth(60); ImGui::InputFloat("##VEC2_1_DATA_X", &m_V2Arr[1].x); ImGui::SameLine(); ImGui::Text(",  Y"); ImGui::SameLine(); ImGui::SetNextItemWidth(60); ImGui::InputFloat("##VEC2_1_DATA_Y", &m_V2Arr[1].y);
		ImGui::Text("VEC2_2"); ImGui::SameLine(100); ImGui::SetNextItemWidth(m_DescSize); ImGui::InputText("##VEC2_2_Desc", (char*)m_V2_Desc[2].data(), 32,ImGuiInputTextFlags_ReadOnly); ImGui::SameLine(); ImGui::SameLine(); ImGui::Text("  X"); ImGui::SameLine(); ImGui::SetNextItemWidth(60); ImGui::InputFloat("##VEC2_2_DATA_X", &m_V2Arr[2].x); ImGui::SameLine(); ImGui::Text(",  Y"); ImGui::SameLine(); ImGui::SetNextItemWidth(60); ImGui::InputFloat("##VEC2_2_DATA_Y", &m_V2Arr[2].y);
		ImGui::Text("VEC2_3"); ImGui::SameLine(100); ImGui::SetNextItemWidth(m_DescSize); ImGui::InputText("##VEC2_3_Desc", (char*)m_V2_Desc[3].data(), 32,ImGuiInputTextFlags_ReadOnly); ImGui::SameLine(); ImGui::SameLine(); ImGui::Text("  X"); ImGui::SameLine(); ImGui::SetNextItemWidth(60); ImGui::InputFloat("##VEC2_3_DATA_X", &m_V2Arr[3].x); ImGui::SameLine(); ImGui::Text(",  Y"); ImGui::SameLine(); ImGui::SetNextItemWidth(60); ImGui::InputFloat("##VEC2_3_DATA_Y", &m_V2Arr[3].y);

		ImGui::Separator(); ImGui::Spacing(); ImGui::Spacing();
	}

	// Vec4_Param
	if (ImGui::CollapsingHeader("VEC4_PARAM", ImGuiTreeNodeFlags_None))
	{
		ImGui::Text("VEC4_0"); ImGui::SameLine(100); ImGui::InputText("##VEC4_0_Desc", (char*)m_V4_Desc[0].data(), 32, ImGuiInputTextFlags_ReadOnly);
		ImGui::Text("Data"); ImGui::SameLine(100); ImGui::InputFloat4("##VEC4_0_DATA", m_V4Arr[0]);
		ImGui::Spacing(); ImGui::Spacing();

		ImGui::Text("VEC4_1"); ImGui::SameLine(100); ImGui::InputText("##VEC4_1_Desc", (char*)m_V4_Desc[1].data(), 32, ImGuiInputTextFlags_ReadOnly);
		ImGui::Text("Data"); ImGui::SameLine(100); ImGui::InputFloat4("##VEC4_1_DATA", m_V4Arr[1]);
		ImGui::Spacing(); ImGui::Spacing();

		ImGui::Text("VEC4_2"); ImGui::SameLine(100); ImGui::InputText("##VEC4_2_Desc", (char*)m_V4_Desc[2].data(), 32, ImGuiInputTextFlags_ReadOnly);
		ImGui::Text("Data"); ImGui::SameLine(100); ImGui::InputFloat4("##VEC4_2_DATA", m_V4Arr[2]);
		ImGui::Spacing(); ImGui::Spacing();

		ImGui::Text("VEC4_3"); ImGui::SameLine(100); ImGui::InputText("##VEC4_3_Desc", (char*)m_V4_Desc[3].data(), 32, ImGuiInputTextFlags_ReadOnly);
		ImGui::Text("Data"); ImGui::SameLine(100); ImGui::InputFloat4("##VEC4_3_DATA", m_V4Arr[3]);
		ImGui::Spacing(); ImGui::Spacing();
		ImGui::Separator(); ImGui::Spacing(); ImGui::Spacing();
	}

	// TEX_0 Param
	if (ImGui::CollapsingHeader("TEX_0_PARAM", ImGuiTreeNodeFlags_None))
	{
		// 해당 텍스쳐 이미지 출력
		Ptr<CTexture> pTex = m_TargetMtrl->GetTexParam(TEX_PARAM::TEX_0);
		ImGui::Text("TEX_0 Path "); ImGui::SameLine(100);
		ImGui::InputText("##TexName", (char*)m_TEX_PATH[0].c_str(), m_TEX_PATH[0].length(), ImGuiInputTextFlags_ReadOnly);

		// Texture Drop 체크
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

			if (payload)
			{
				DWORD_PTR data = *((DWORD_PTR*)payload->Data);
				CAsset* pAsset = (CAsset*)data;
				if (ASSET_TYPE::TEXTURE == pAsset->GetType())
				{
					m_TargetMtrl->SetTexParam(TEX_PARAM::TEX_0, (CTexture*)pAsset);
					m_TEX_PATH[0] = WstrToStr(m_TargetMtrl->GetTexParam(TEX_PARAM::TEX_0)->GetKey());
				}
			}

			ImGui::EndDragDropTarget();
		}

		// List UI
		ImGui::SameLine();
		if (ImGui::Button("##MatTex0", ImVec2(20, 20)))
		{
			UIListPannel* pListUI = (UIListPannel*)CImGuiMgr::GetInst()->FindUI("##List");
			vector<string> vecTex;
			unordered_map<wstring, Ptr<CAsset>> hashTex = CAssetMgr::GetInst()->GetAssetsByType(ASSET_TYPE::TEXTURE);
			for (const std::pair<wstring, Ptr<CAsset>>& pair : hashTex)
				vecTex.push_back(WstrToStr(pair.first));

			pListUI->AddString(vecTex);
			pListUI->SetDBClickeDelegate(this, (DELEGATE_1)&UIMatEditor::SelectTex);
			pListUI->Activate();
		}

		ImGui::Separator(); ImGui::Spacing(); ImGui::Spacing();
		if (pTex != nullptr)
		{
			ImGui::Text("TEX_0 Desc"); ImGui::SameLine(100); ImGui::InputText("##TEX_0_Desc", (char*)m_TEX_Desc[0].data(), 32);

			// 해상도 정보 출력
			float Width = (float)pTex->GetWidth();
			float Height = (float)pTex->GetHeight();

			ImGui::Text("Width"); ImGui::SameLine(100); ImGui::InputFloat("##TEX_0_Width", &Width, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_ReadOnly);
			ImGui::Text("Height"); ImGui::SameLine(100); ImGui::InputFloat("##TEX_0_Height", &Height, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_ReadOnly);

			static bool use_text_color_for_tint = false;
			ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
			ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
			ImVec4 tint_col = use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
			ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
			ImGui::Image(pTex->GetSRV().Get(), ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);
		}
	}

	// TEX_1 Param
	if (ImGui::CollapsingHeader("TEX_1_PARAM", ImGuiTreeNodeFlags_None))
	{
		// 해당 텍스쳐 이미지 출력
		Ptr<CTexture> pTex = m_TargetMtrl->GetTexParam(TEX_PARAM::TEX_1);
		ImGui::Text("TEX_1 Path "); ImGui::SameLine(100);
		ImGui::InputText("##TexName", (char*)m_TEX_PATH[1].c_str(), m_TEX_PATH[1].length(), ImGuiInputTextFlags_ReadOnly);

		// Texture Drop 체크
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

			if (payload)
			{
				DWORD_PTR data = *((DWORD_PTR*)payload->Data);
				CAsset* pAsset = (CAsset*)data;
				if (ASSET_TYPE::TEXTURE == pAsset->GetType())
				{
					m_TargetMtrl->SetTexParam(TEX_PARAM::TEX_1, (CTexture*)pAsset);
					m_TEX_PATH[1] = WstrToStr(m_TargetMtrl->GetTexParam(TEX_PARAM::TEX_1)->GetKey());
				}
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::Separator(); ImGui::Spacing(); ImGui::Spacing();
		if (pTex != nullptr)
		{
			ImGui::Text("TEX_1 Desc"); ImGui::SameLine(100); ImGui::InputText("##TEX_1_Desc", (char*)m_TEX_Desc[1].data(), 32);

			// 해상도 정보 출력
			float Width = (float)pTex->GetWidth();
			float Height = (float)pTex->GetHeight();

			ImGui::Text("Width"); ImGui::SameLine(100); ImGui::InputFloat("##TEX_1_Width", &Width, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_ReadOnly);
			ImGui::Text("Height"); ImGui::SameLine(100); ImGui::InputFloat("##TEX_1_Height", &Height, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_ReadOnly);

			static bool use_text_color_for_tint = false;
			ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
			ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
			ImVec4 tint_col = use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
			ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
			ImGui::Image(pTex->GetSRV().Get(), ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);
		}
	}

	// TEX_2 Param
	if (ImGui::CollapsingHeader("TEX_2_PARAM", ImGuiTreeNodeFlags_None))
	{
		// 해당 텍스쳐 이미지 출력
		Ptr<CTexture> pTex = m_TargetMtrl->GetTexParam(TEX_PARAM::TEX_2);
		ImGui::Text("TEX_2 Path "); ImGui::SameLine(100);
		ImGui::InputText("##TexName", (char*)m_TEX_PATH[2].c_str(), m_TEX_PATH[2].length(), ImGuiInputTextFlags_ReadOnly);

		// Texture Drop 체크
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

			if (payload)
			{
				DWORD_PTR data = *((DWORD_PTR*)payload->Data);
				CAsset* pAsset = (CAsset*)data;
				if (ASSET_TYPE::TEXTURE == pAsset->GetType())
				{
					m_TargetMtrl->SetTexParam(TEX_PARAM::TEX_2, (CTexture*)pAsset);
					m_TEX_PATH[2] = WstrToStr(m_TargetMtrl->GetTexParam(TEX_PARAM::TEX_2)->GetKey());
				}
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::Separator(); ImGui::Spacing(); ImGui::Spacing();
		if (pTex != nullptr)
		{
			ImGui::Text("TEX_2 Desc"); ImGui::SameLine(100); ImGui::InputText("##TEX_2_Desc", (char*)m_TEX_Desc[2].data(), 32);

			// 해상도 정보 출력
			float Width = (float)pTex->GetWidth();
			float Height = (float)pTex->GetHeight();

			ImGui::Text("Width"); ImGui::SameLine(100); ImGui::InputFloat("##TEX_2_Width", &Width, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_ReadOnly);
			ImGui::Text("Height"); ImGui::SameLine(100); ImGui::InputFloat("##TEX_2_Height", &Height, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_ReadOnly);

			static bool use_text_color_for_tint = false;
			ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
			ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
			ImVec4 tint_col = use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
			ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
			ImGui::Image(pTex->GetSRV().Get(), ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);
		}
	}

	// TEX_3 Param
	if (ImGui::CollapsingHeader("TEX_3_PARAM", ImGuiTreeNodeFlags_None))
	{
		// 해당 텍스쳐 이미지 출력
		Ptr<CTexture> pTex = m_TargetMtrl->GetTexParam(TEX_PARAM::TEX_3);
		ImGui::Text("TEX_3 Path "); ImGui::SameLine(100);
		ImGui::InputText("##TexName", (char*)m_TEX_PATH[3].c_str(), m_TEX_PATH[3].length(), ImGuiInputTextFlags_ReadOnly);

		// Texture Drop 체크
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

			if (payload)
			{
				DWORD_PTR data = *((DWORD_PTR*)payload->Data);
				CAsset* pAsset = (CAsset*)data;
				if (ASSET_TYPE::TEXTURE == pAsset->GetType())
				{
					m_TargetMtrl->SetTexParam(TEX_PARAM::TEX_3, (CTexture*)pAsset);
					m_TEX_PATH[3] = WstrToStr(m_TargetMtrl->GetTexParam(TEX_PARAM::TEX_3)->GetKey());
				}
			}

			ImGui::EndDragDropTarget();
		}
		ImGui::Separator(); ImGui::Spacing(); ImGui::Spacing();
		if (pTex != nullptr)
		{
			ImGui::Text("TEX_3 Desc"); ImGui::SameLine(100); ImGui::InputText("##TEX_3_Desc", (char*)m_TEX_Desc[3].data(), 32);

			// 해상도 정보 출력
			float Width = (float)pTex->GetWidth();
			float Height = (float)pTex->GetHeight();

			ImGui::Text("Width"); ImGui::SameLine(100); ImGui::InputFloat("##TEX_3_Width", &Width, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_ReadOnly);
			ImGui::Text("Height"); ImGui::SameLine(100); ImGui::InputFloat("##TEX_3_Height", &Height, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_ReadOnly);

			static bool use_text_color_for_tint = false;
			ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
			ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
			ImVec4 tint_col = use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
			ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
			ImGui::Image(pTex->GetSRV().Get(), ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);
		}
	}

	// TEX_4 Param
	if (ImGui::CollapsingHeader("TEX_4_PARAM", ImGuiTreeNodeFlags_None))
	{
		// 해당 텍스쳐 이미지 출력
		Ptr<CTexture> pTex = m_TargetMtrl->GetTexParam(TEX_PARAM::TEX_4);
		ImGui::Text("TEX_4 Path "); ImGui::SameLine(100);
		ImGui::InputText("##TexName", (char*)m_TEX_PATH[4].c_str(), m_TEX_PATH[4].length(), ImGuiInputTextFlags_ReadOnly);

		// Texture Drop 체크
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

			if (payload)
			{
				DWORD_PTR data = *((DWORD_PTR*)payload->Data);
				CAsset* pAsset = (CAsset*)data;
				if (ASSET_TYPE::TEXTURE == pAsset->GetType())
				{
					m_TargetMtrl->SetTexParam(TEX_PARAM::TEX_4, (CTexture*)pAsset);
					m_TEX_PATH[4] = WstrToStr(m_TargetMtrl->GetTexParam(TEX_PARAM::TEX_4)->GetKey());
				}
			}

			ImGui::EndDragDropTarget();
		}
		ImGui::Separator(); ImGui::Spacing(); ImGui::Spacing();
		if (pTex != nullptr)
		{
			ImGui::Text("TEX_4 Desc"); ImGui::SameLine(100); ImGui::InputText("##TEX_4_Desc", (char*)m_TEX_Desc[4].data(), 32);

			// 해상도 정보 출력
			float Width = (float)pTex->GetWidth();
			float Height = (float)pTex->GetHeight();

			ImGui::Text("Width"); ImGui::SameLine(100); ImGui::InputFloat("##TEX_4_Width", &Width, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_ReadOnly);
			ImGui::Text("Height"); ImGui::SameLine(100); ImGui::InputFloat("##TEX_4_Height", &Height, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_ReadOnly);

			static bool use_text_color_for_tint = false;
			ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
			ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
			ImVec4 tint_col = use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
			ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
			ImGui::Image(pTex->GetSRV().Get(), ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);
		}
	}

	// TEX_5 Param
	if (ImGui::CollapsingHeader("TEX_5_PARAM", ImGuiTreeNodeFlags_None))
	{
		// 해당 텍스쳐 이미지 출력
		Ptr<CTexture> pTex = m_TargetMtrl->GetTexParam(TEX_PARAM::TEX_5);
		ImGui::Text("TEX_5 Path "); ImGui::SameLine(100);
		ImGui::InputText("##TexName", (char*)m_TEX_PATH[5].c_str(), m_TEX_PATH[5].length(), ImGuiInputTextFlags_ReadOnly);

		// Texture Drop 체크
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

			if (payload)
			{
				DWORD_PTR data = *((DWORD_PTR*)payload->Data);
				CAsset* pAsset = (CAsset*)data;
				if (ASSET_TYPE::TEXTURE == pAsset->GetType())
				{
					m_TargetMtrl->SetTexParam(TEX_PARAM::TEX_5, (CTexture*)pAsset);
					m_TEX_PATH[5] = WstrToStr(m_TargetMtrl->GetTexParam(TEX_PARAM::TEX_5)->GetKey());
				}
			}

			ImGui::EndDragDropTarget();
		}
		ImGui::Separator(); ImGui::Spacing(); ImGui::Spacing();
		if (pTex != nullptr)
		{
			ImGui::Text("TEX_5 Desc"); ImGui::SameLine(100); ImGui::InputText("##TEX_5_Desc", (char*)m_TEX_Desc[5].data(), 32);

			// 해상도 정보 출력
			float Width = (float)pTex->GetWidth();
			float Height = (float)pTex->GetHeight();

			ImGui::Text("Width"); ImGui::SameLine(100); ImGui::InputFloat("##TEX_5_Width", &Width, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_ReadOnly);
			ImGui::Text("Height"); ImGui::SameLine(100); ImGui::InputFloat("##TEX_5_Height", &Height, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_ReadOnly);

			static bool use_text_color_for_tint = false;
			ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
			ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
			ImVec4 tint_col = use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
			ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
			ImGui::Image(pTex->GetSRV().Get(), ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);
		}
	}
}

void UIMatEditor::SelectTex(DWORD_PTR _ptr)
{
	string strTexKey = (char*)_ptr;
	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(StrToWstr(strTexKey));
	if (!pTex.Get())
		return;

	m_TargetMtrl->SetTexParam(TEX_PARAM::TEX_0, pTex);
	m_TEX_PATH[0] = strTexKey;
}

void UIMatEditor::Save()
{
	// Target Mtrl이 없다면 return
	if (nullptr == m_TargetMtrl)
		return;

	// 경로 설정
	wchar_t szPath[255] = {};
	wstring FilePath = CPathMgr::GetContentPath();
	wstring MtrlPath = StrToWstr(m_MtrlPath);
	swprintf_s(szPath, L"material\\%s.mat", MtrlPath.c_str());

	// 중복 체크
	if (exists(FilePath + szPath))
	{
		int value = MessageBoxA(nullptr, "머터리얼이 이미 존재합니다. \n덮어쓰시겠습니까?", "Material Already Exist!", MB_YESNO);
		if (value != IDYES)
			return;
	}

	// 경로 저장
	m_TargetMtrl->SetName(szPath);

	// Scarla Param Save
	// INT_PARAM
	m_TargetMtrl->SetScalarParam(SCALAR_PARAM::INT_0, m_IntArr[0]);
	m_TargetMtrl->SetScalarParam(SCALAR_PARAM::INT_1, m_IntArr[1]);
	m_TargetMtrl->SetScalarParam(SCALAR_PARAM::INT_2, m_IntArr[2]);
	m_TargetMtrl->SetScalarParam(SCALAR_PARAM::INT_3, m_IntArr[3]);

	// FLOAT_PARAM
	m_TargetMtrl->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_FloatArr[0]);
	m_TargetMtrl->SetScalarParam(SCALAR_PARAM::FLOAT_1, m_FloatArr[1]);
	m_TargetMtrl->SetScalarParam(SCALAR_PARAM::FLOAT_2, m_FloatArr[2]);
	m_TargetMtrl->SetScalarParam(SCALAR_PARAM::FLOAT_3, m_FloatArr[3]);

	// VEC2_PARAM
	m_TargetMtrl->SetScalarParam(SCALAR_PARAM::VEC2_0, m_V2Arr[0]);
	m_TargetMtrl->SetScalarParam(SCALAR_PARAM::VEC2_1, m_V2Arr[1]);
	m_TargetMtrl->SetScalarParam(SCALAR_PARAM::VEC2_2, m_V2Arr[2]);
	m_TargetMtrl->SetScalarParam(SCALAR_PARAM::VEC2_3, m_V2Arr[3]);

	// VEC4_PARAM
	m_TargetMtrl->SetScalarParam(SCALAR_PARAM::VEC4_0, m_V4Arr[0]);
	m_TargetMtrl->SetScalarParam(SCALAR_PARAM::VEC4_1, m_V4Arr[1]);
	m_TargetMtrl->SetScalarParam(SCALAR_PARAM::VEC4_2, m_V4Arr[2]);
	m_TargetMtrl->SetScalarParam(SCALAR_PARAM::VEC4_3, m_V4Arr[3]);

	m_TargetMtrl->Save(szPath);

	Reset();
}

void UIMatEditor::Load()
{
	Reset();

	wchar_t szSelect[256] = {};

	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFile = szSelect;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szSelect);
	ofn.lpstrFilter = L"ALL\0*.*\0material\0*.mat";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;

	// 탐색창 초기 위치 지정
	wstring strInitPath = CPathMgr::GetContentPath();
	strInitPath += L"material\\";
	ofn.lpstrInitialDir = strInitPath.c_str();

	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// 파일 선택
	if (GetOpenFileName(&ofn))
	{
		// 파일이름 분석
		wchar_t szFileName[128] = {};
		wchar_t szExt[16] = {};
		_wsplitpath_s(ofn.lpstrFile, nullptr, 0, nullptr, 0, szFileName, 128, szExt, 16);
		wstring relativePath = CPathMgr::GetRelativePath(ofn.lpstrFile);
		wstring strExt = (wstring)szExt;

		if (strExt == L".mat" || strExt == L".mtrl")
		{
			// save
			m_TargetMtrl = CAssetMgr::GetInst()->Load<CMaterial>(relativePath);
			m_MtrlPath = WstrToStr(m_TargetMtrl->GetKey());

			// find file name
			m_MtrlPath = path(m_MtrlPath).filename().stem().string();

			if (m_TargetMtrl->GetShader() != nullptr)
				m_ShaderName = WstrToStr(m_TargetMtrl->GetShader()->GetKey());
		}
	}

	// 파리미터 로딩
	if (nullptr == m_TargetMtrl)
		return;

	// INT
	m_IntArr[0] = *((int*)m_TargetMtrl->GetScalarParam(SCALAR_PARAM::INT_0));
	m_IntArr[1] = *((int*)m_TargetMtrl->GetScalarParam(SCALAR_PARAM::INT_1));
	m_IntArr[2] = *((int*)m_TargetMtrl->GetScalarParam(SCALAR_PARAM::INT_2));
	m_IntArr[3] = *((int*)m_TargetMtrl->GetScalarParam(SCALAR_PARAM::INT_3));

	// FLOAT
	m_FloatArr[0] = *((float*)m_TargetMtrl->GetScalarParam(SCALAR_PARAM::FLOAT_0));
	m_FloatArr[1] = *((float*)m_TargetMtrl->GetScalarParam(SCALAR_PARAM::FLOAT_1));
	m_FloatArr[2] = *((float*)m_TargetMtrl->GetScalarParam(SCALAR_PARAM::FLOAT_2));
	m_FloatArr[3] = *((float*)m_TargetMtrl->GetScalarParam(SCALAR_PARAM::FLOAT_3));

	// Vec2
	m_V2Arr[0] = *((Vec2*)m_TargetMtrl->GetScalarParam(SCALAR_PARAM::VEC2_0));
	m_V2Arr[1] = *((Vec2*)m_TargetMtrl->GetScalarParam(SCALAR_PARAM::VEC2_1));
	m_V2Arr[2] = *((Vec2*)m_TargetMtrl->GetScalarParam(SCALAR_PARAM::VEC2_2));
	m_V2Arr[3] = *((Vec2*)m_TargetMtrl->GetScalarParam(SCALAR_PARAM::VEC2_3));

	// Vec4
	Vec4 V4_Param = *((Vec4*)m_TargetMtrl->GetScalarParam(SCALAR_PARAM::VEC4_0));

	m_V4Arr[0][0] = V4_Param.x;
	m_V4Arr[0][1] = V4_Param.y;
	m_V4Arr[0][2] = V4_Param.z;
	m_V4Arr[0][3] = V4_Param.w;

	V4_Param = *((Vec4*)m_TargetMtrl->GetScalarParam(SCALAR_PARAM::VEC4_1));

	m_V4Arr[1][0] = V4_Param.x;
	m_V4Arr[1][1] = V4_Param.y;
	m_V4Arr[1][2] = V4_Param.z;
	m_V4Arr[1][3] = V4_Param.w;

	V4_Param = *((Vec4*)m_TargetMtrl->GetScalarParam(SCALAR_PARAM::VEC4_2));

	m_V4Arr[2][0] = V4_Param.x;
	m_V4Arr[2][1] = V4_Param.y;
	m_V4Arr[2][2] = V4_Param.z;
	m_V4Arr[2][3] = V4_Param.w;

	V4_Param = *((Vec4*)m_TargetMtrl->GetScalarParam(SCALAR_PARAM::VEC4_3));

	m_V4Arr[3][0] = V4_Param.x;
	m_V4Arr[3][1] = V4_Param.y;
	m_V4Arr[3][2] = V4_Param.z;
	m_V4Arr[3][3] = V4_Param.w;
	
	// Path 로딩
	for (int i = 0; i <= (UINT)TEX_PARAM::TEX_5; ++i)
	{
		TEX_PARAM CurParam = (TEX_PARAM)i;

		if (m_TargetMtrl->GetTexParam(CurParam) != nullptr)
		{
			m_TEX_PATH[i] = WstrToStr(m_TargetMtrl->GetTexParam(CurParam)->GetKey());
		}
	}

	// 디스크립션 로딩
	Ptr<CGraphicsShader> pShader = m_TargetMtrl->GetShader();

	if (nullptr != pShader)
	{
		const vector<FScalarParam>& ScarlaParam = pShader->GetScalarParam();
		for (int i = 0; i < ScarlaParam.size(); ++i)
		{
			switch (ScarlaParam[i].Type)
			{
			case SCALAR_PARAM::INT_0:
				m_INT_Desc[0] = ScarlaParam[i].Desc;
				break;
			case SCALAR_PARAM::INT_1:
				m_INT_Desc[1] = ScarlaParam[i].Desc;
				break;
			case SCALAR_PARAM::INT_2:
				m_INT_Desc[2] = ScarlaParam[i].Desc;
				break;
			case SCALAR_PARAM::INT_3:
				m_INT_Desc[3] = ScarlaParam[i].Desc;
				break;
			case SCALAR_PARAM::FLOAT_0:
				m_FLOAT_Desc[0] = ScarlaParam[i].Desc;
				break;
			case SCALAR_PARAM::FLOAT_1:
				m_FLOAT_Desc[1] = ScarlaParam[i].Desc;
				break;
			case SCALAR_PARAM::FLOAT_2:
				m_FLOAT_Desc[2] = ScarlaParam[i].Desc;
				break;
			case SCALAR_PARAM::FLOAT_3:
				m_FLOAT_Desc[3] = ScarlaParam[i].Desc;
				break;
			case SCALAR_PARAM::VEC2_0:
				m_V2_Desc[0] = ScarlaParam[i].Desc;
				break;
			case SCALAR_PARAM::VEC2_1:
				m_V2_Desc[1] = ScarlaParam[i].Desc;
				break;
			case SCALAR_PARAM::VEC2_2:
				m_V2_Desc[2] = ScarlaParam[i].Desc;
				break;
			case SCALAR_PARAM::VEC2_3:
				m_V2_Desc[3] = ScarlaParam[i].Desc;
				break;
			case SCALAR_PARAM::VEC4_0:
				m_V4_Desc[0] = ScarlaParam[i].Desc;
				break;
			case SCALAR_PARAM::VEC4_1:
				m_V4_Desc[1] = ScarlaParam[i].Desc;
				break;
			case SCALAR_PARAM::VEC4_2:
				m_V4_Desc[2] = ScarlaParam[i].Desc;
				break;
			case SCALAR_PARAM::VEC4_3:
				m_V4_Desc[3] = ScarlaParam[i].Desc;
				break;
			default:
				break;
			}
		}

		const vector<FTexParam>& TexParam = pShader->GetTexParam();

		for (int i = 0; i < TexParam.size(); ++i)
		{
			switch (TexParam[i].Type)
			{
			case TEX_PARAM::TEX_0:
				m_TEX_Desc[0] = TexParam[i].Desc;
				break;
			case TEX_PARAM::TEX_1:
				m_TEX_Desc[1] = TexParam[i].Desc;
				break;
			case TEX_PARAM::TEX_2:
				m_TEX_Desc[2] = TexParam[i].Desc;
				break;
			case TEX_PARAM::TEX_3:
				m_TEX_Desc[3] = TexParam[i].Desc;
				break;
			case TEX_PARAM::TEX_4:
				m_TEX_Desc[4] = TexParam[i].Desc;
				break;
			case TEX_PARAM::TEX_5:
				m_TEX_Desc[5] = TexParam[i].Desc;
				break;
			default:
				break;
			}
		}
	}
}

void UIMatEditor::Create()
{
	Reset();

	Ptr<CMaterial> pMtrl = new CMaterial(false);
	m_TargetMtrl = pMtrl;
}

void UIMatEditor::Reset()
{
	m_TargetMtrl = nullptr;
	m_MtrlPath = "New Material";

	char Desc_Name[255] = {};

	// INT_DESC
	for (int i = 0; i < 4; ++i)
	{
		// 문자열 초기화
		memset(Desc_Name, 0, sizeof(Desc_Name));
		sprintf_s(Desc_Name, "INT_%d_DESC", i);
		m_INT_Desc[i] = Desc_Name;
	}

	// FLOAT_DESC
	for (int i = 0; i < 4; ++i)
	{
		// 문자열 초기화
		memset(Desc_Name, 0, sizeof(Desc_Name));
		sprintf_s(Desc_Name, "FLOAT_%d_DESC", i);
		m_FLOAT_Desc[i] = Desc_Name;
	}

	// Vec2_DESC
	for (int i = 0; i < 4; ++i)
	{
		// 문자열 초기화
		memset(Desc_Name, 0, sizeof(Desc_Name));
		sprintf_s(Desc_Name, "VEC2_%d_DESC", i);
		m_V2_Desc[i] = Desc_Name;
	}

	// Vec4_DESC
	for (int i = 0; i < 4; ++i)
	{
		// 문자열 초기화
		memset(Desc_Name, 0, sizeof(Desc_Name));
		sprintf_s(Desc_Name, "VEC4_%d_DESC", i);
		m_V4_Desc[i] = Desc_Name;
	}

	// TEX_DESC m_TEX_Desc
	for (int i = 0; i < 6; ++i)
	{
		// 문자열 초기화
		memset(Desc_Name, 0, sizeof(Desc_Name));
		sprintf_s(Desc_Name, "TEX_%d", i);
		m_TEX_Desc[i] = Desc_Name;
	}
}

