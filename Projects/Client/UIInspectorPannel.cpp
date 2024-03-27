#include "pch.h"
#include "UIInspectorPannel.h"
#include "func_ImGUI.h"
#include "CImGuiMgr.h"
#include "UITransform.h"
#include "UIMeshRender.h"
#include "UIAsset.h"
#include "UIScript.h"

#include <Engine/CTransform.h>

UIInspectorPannel::UIInspectorPannel()
	: UI("Inspector", "##Inspector")
	, m_TargetObject(nullptr)
	, m_arrCompUI{}
{
	// 자식 UI 생성
	CreateComponentUI();
	CreateAssetUI();
}

UIInspectorPannel::~UIInspectorPannel()
{
}

void UIInspectorPannel::tick()
{
}

void UIInspectorPannel::render_update()
{
	if (!m_TargetObject)
		return;
	
	string strName = "[ GameObject: " + string(m_TargetObject->GetName().begin(), m_TargetObject->GetName().end()) + " ]";
	ImGui::Text(strName.c_str());

	TextBox("Layer"); ImGui::SameLine();
	DrawLayerUI();
	ImGui::Separator();
}

void UIInspectorPannel::SetTargetObject(CGameObject* _Object)
{
	m_TargetObject = _Object;

	for (size_t i = 0; i < (size_t)COMPONENT_TYPE::END; ++i)
	{
		if (m_arrCompUI[i])
		{
			m_arrCompUI[i]->SetTargetObject(_Object);
		}
	}

	if (not _Object)
	{
		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			m_vecScriptUI[i]->Deactivate();
		}
	}
	else
	{
		const vector<CScript*>& vecScripts = _Object->GetScripts();
		ResizeScriptVec((UINT)vecScripts.size());

		for (size_t i = 0; i < vecScripts.size(); ++i)
		{
			m_vecScriptUI[i]->SetTargetObject(_Object);
			m_vecScriptUI[i]->SetScript(vecScripts[i]);
		}
	}

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		if (m_arrAssetUI[i])
			m_arrAssetUI[i]->Deactivate();
	}
}

void UIInspectorPannel::SetTargetAsset(Ptr<CAsset> _Asset)
{
	SetTargetObject(nullptr);
	m_TargetAsset = _Asset;

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
		m_arrAssetUI[i]->Deactivate();

	if (m_TargetAsset.Get())
	{
		m_arrAssetUI[(UINT)m_TargetAsset->GetType()]->Activate();
		m_arrAssetUI[(UINT)m_TargetAsset->GetType()]->SetAsset(_Asset);
	}
}

void UIInspectorPannel::DrawLayerUI()
{
	ImGui::BeginGroup();
	{
		const vector<string>& LayerName = CImGuiMgr::GetInst()->GetLayerName();
		int item_current_idx = m_TargetObject->GetLayerIdx();
		int item_prev_idx = item_current_idx;
		if (ImGui::BeginCombo("##CheckLayerList", LayerName[item_current_idx].c_str()))
		{
			for (int i = 0; i < LayerName.size(); i++)
			{
				const bool is_selected = (item_current_idx == i);

				if (ImGui::Selectable(LayerName[i].c_str(), is_selected))
					item_current_idx = i;

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (item_prev_idx != item_current_idx)
			m_TargetObject->ChangeLayer(item_current_idx);
		item_prev_idx = item_current_idx;

		ImGui::EndGroup();
	}
}

void UIInspectorPannel::ResizeScriptVec(UINT _Size)
{
	if (m_vecScriptUI.size() > _Size)
	{
		for (int i = _Size; i < m_vecScriptUI.size(); ++i)
		{
			//m_vecScriptUI[i] = nullptr;
			m_vecScriptUI[i]->Deactivate();
		}
	}

	while (m_vecScriptUI.size() < _Size)
	{
		UIScript* pScriptUI = new UIScript;
		m_vecScriptUI.push_back(pScriptUI);
		AddChild(pScriptUI);
	}
}