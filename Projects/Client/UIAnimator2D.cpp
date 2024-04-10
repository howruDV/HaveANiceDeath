#include "pch.h"
#include "func_ImGUI.h"
#include "UIAnimator2D.h"
#include "CImGuiMgr.h"

#include <Engine/CAnimator2D.h>

UIAnimator2D::UIAnimator2D()
	: UIComponent("Animator2D", "##Animator2D", COMPONENT_TYPE::ANIMATOR2D)
{
}

UIAnimator2D::~UIAnimator2D()
{
}

void UIAnimator2D::render_update()
{
	if (not GetTargetObject())
		return;

	UIComponent::render_update();

	string CurAnim = WstrToStr(GetTargetObject()->Animator2D()->GetCurAnimName());
	const unordered_map<wstring, CAnim*> CurAnimList = GetTargetObject()->Animator2D()->GetAnimations();
	vector<string> items;
	static int item_current_idx = 0;
	items.reserve(CurAnimList.size());

	for (const auto& iter : CurAnimList)
	{
		items.push_back(WstrToStr(iter.first).c_str());
	}

	// 현재 애니메이션
	TextBox("Current Animation"); ImGui::SameLine();
	ImGui::InputText("##CurrentAnimation", (char*)CurAnim.c_str(), CurAnim.length(), ImGuiInputTextFlags_ReadOnly);

	char buffer[64];
	strcpy_s(buffer, m_AddStateKey.c_str());
	TextBox("New Anim Key"); ImGui::SameLine(); ImGui::InputText("##AnimationKey", buffer, 64);
	m_AddStateKey = buffer;

	// 애니메이션 목록
	TextBox("Animation List");
	ImGui::SameLine();

	ImGui::BeginGroup();
	{
		if (ImGui::BeginListBox("##AnimationList", ImVec2(ImGui::CalcItemWidth(), 5 * ImGui::GetTextLineHeightWithSpacing())))
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

		// Btn
		ImVec2 buttonSize{ 155.f, 25.f };
		float posX = (ImGui::GetCursorPos().x + ImGui::CalcItemWidth()) -  buttonSize.x * 2 - ImGui::GetStyle().ItemSpacing.x;
		ImGui::SetCursorPosX(posX);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 0)); // 버튼 간 간격 조정

		if (ImGui::Button("Add##AddAnimation", buttonSize))
		{
			AddAnimation();
		}

		ImGui::SameLine();

		if (ImGui::Button("Delete##DeleteAnimation", buttonSize))
		{
			wstring SelectedAnimKey = StrToWstr(items[item_current_idx]);
			DeleteAnimation(SelectedAnimKey);
		}

		ImGui::PopStyleVar();
		ImGui::EndGroup();
	}
}

void UIAnimator2D::AddAnimation()
{
	// Animation 파일 가져와서 저장
	wchar_t szSelect[256] = {};

	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFile = szSelect;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szSelect);
	ofn.lpstrFilter = L"ALL\0*.*\0Anim\0*.anim";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;

	// 탐색창 초기 위치 지정
	wstring strInitPath = CPathMgr::GetContentPath();
	strInitPath += L"animation\\";
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

		// open read file
		FILE* pFile = nullptr;
		_wfopen_s(&pFile, (CPathMgr::GetContentPath() + relativePath).c_str(), L"rb");

		// read
		CAnim* pAnim = new CAnim;
		pAnim->LoadFromFile(pFile);
		GetTargetObject()->Animator2D()->Create(pAnim, StrToWstr(m_AddStateKey));

		// close read file
		fclose(pFile);
	}
}

void UIAnimator2D::DeleteAnimation(const wstring& _strAnimationName)
{
	GetTargetObject()->Animator2D()->DeleteAnim(_strAnimationName);
}
