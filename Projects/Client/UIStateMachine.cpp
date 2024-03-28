#include "pch.h"
#include "UIStateMachine.h"
#include "UIListPannel.h"
#include "func_ImGUI.h"

#include <Engine\CStateMachine.h>
#include <Engine\CFSM.h>
#include <Engine\CState.h>

#include <States\CStateMgr.h>

UIStateMachine::UIStateMachine()
	: UIComponent("StateMachine", "##StateMachine", COMPONENT_TYPE::STATEMACHINE)
{
}

UIStateMachine::~UIStateMachine()
{
}

void UIStateMachine::StateSelect(DWORD_PTR _ptr)
{
	string strState = (char*)_ptr;
	wstring strStateName = StrToWstr(strState);

	if (m_AddStateKey == "")
		return;

	CState* pState = CStateMgr::GetState(strStateName);
	wstring AddKey = StrToWstr(m_AddStateKey);

	Ptr<CFSM> pFSM = GetTargetObject()->StateMachine()->GetFSMOrigin();

	if (pFSM->FindState(AddKey) == nullptr)
	{
		pFSM->AddState(AddKey, pState);
	}
	else
	{
		MessageBoxA(nullptr, "State의 Key가 이미 존재합니다.", "Add State Failed!", MB_OK);
		delete pState;
	}
}

void UIStateMachine::render_update()
{
	if (nullptr == GetTargetObject())
		return;

	UIComponent::render_update();

	CGameObject* pTarget = GetTargetObject();
	CStateMachine* pStateMachine = pTarget->StateMachine();
	Ptr<CFSM> pFSM = pStateMachine->GetFSMOrigin();
	string FSMName = "";

	if (nullptr != pFSM)
	{
		FSMName = WstrToStr(pFSM->GetKey()).c_str();
	}

	TextBox("FSM Key"); ImGui::SameLine();	ImGui::InputText("##FSMName", (char*)FSMName.c_str(), FSMName.length(), ImGuiInputTextFlags_ReadOnly);
	
	// Drop 체크
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (payload)
		{
			DWORD_PTR data = *((DWORD_PTR*)payload->Data);
			CAsset* pAsset = (CAsset*)data;
			if (ASSET_TYPE::FSM == pAsset->GetType())
			{
				GetTargetObject()->StateMachine()->SetFSM((CFSM*)pAsset);
			}
		}
		ImGui::EndDragDropTarget();
	}

	TextBox("State Name"); ImGui::SameLine(); ImGui::InputText("##StateName", (char*)m_StateName.c_str(), m_StateName.length(), ImGuiInputTextFlags_ReadOnly);

	vector<string> items;

	if (pFSM.Get())
	{
		const unordered_map<wstring, CState*>& States = pFSM->GetStates();

		items.reserve(States.size());

		for (const auto& iter : States)
		{
			items.push_back(WstrToStr(iter.first).c_str());
		}
	}

	char buffer[64];
	strcpy_s(buffer, m_AddStateKey.c_str());
	TextBox("Input Key"); ImGui::SameLine(); ImGui::InputText("##StatesKey", buffer, 64);
	m_AddStateKey = buffer;

	static int item_current_idx = 0; // Here we store our selection data as an index.
	if (ImGui::BeginListBox("##StatesList", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
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

	// 현재 선택된 스테이트의 클래스명을 보여 줌
	if (pFSM.Get() && items.size() > item_current_idx)
	{
		CState* SelectState = pFSM->FindState(StrToWstr(items[item_current_idx]));
		wstring StateName = CStateMgr::GetStateName(SelectState);
		m_StateName = WstrToStr(StateName);
		m_CurStateKey = StrToWstr(items[item_current_idx]);
	}
	else
	{
		m_StateName = "";
	}

	// 버튼 너비 설정
	float buttonWidth = 150.0f;

	// 버튼을 오른쪽 정렬하여 배치
	float posX = ImGui::GetWindowWidth() - buttonWidth * 3 - ImGui::GetStyle().ItemSpacing.x*2;
	ImGui::SetCursorPosX(posX);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 0)); // 버튼 간 간격 조정

	if (ImGui::Button("Add State", ImVec2(buttonWidth, 0)))
	{
		// 리스트 UI
		UIListPannel* pListUI = (UIListPannel*)CImGuiMgr::GetInst()->FindUI("##List");

		vector<wstring> vecStateName;
		CStateMgr::GetStateInfo(vecStateName);

		vector<string> vecStateList;

		for (int i = 0; i < vecStateName.size(); ++i)
		{
			vecStateList.push_back(WstrToStr(vecStateName[i]));
		}

		pListUI->AddString(vecStateList);
		pListUI->SetDBClickeDelegate(this, (DELEGATE_1)&UIStateMachine::StateSelect);
		pListUI->Activate();
	}

	ImGui::SameLine(0, ImGui::GetStyle().ItemSpacing.x);
	posX = ImGui::GetWindowWidth() - buttonWidth * 2 - ImGui::GetStyle().ItemSpacing.x;
	ImGui::SetCursorPosX(posX);

	if (ImGui::Button("Delete State", ImVec2(buttonWidth, 0)))
	{
		pFSM->DeleteState(m_CurStateKey);
	}

	ImGui::SameLine(0, ImGui::GetStyle().ItemSpacing.x);
	posX = ImGui::GetWindowWidth() - buttonWidth;
	ImGui::SetCursorPosX(posX);

	if (ImGui::Button("Save", ImVec2(buttonWidth, 0)))
	{
		if (pFSM.Get())
		{
			// 경로 설정
			wchar_t szPath[255] = {};
			wstring FilePath = CPathMgr::GetContentPath();
			wstring FSMPath = pFSM->GetRelativePath();
			swprintf_s(szPath, L"%s", FSMPath.c_str());


			// 경로 저장
			pFSM->SetName(szPath);

			pFSM->Save(szPath);
			MessageBoxA(nullptr, "FSM이 저장되었습니다.", "Save successed!", MB_OK);
		}
	}

	ImGui::PopStyleVar();
}