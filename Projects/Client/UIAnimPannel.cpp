#include "pch.h"
#include "imgui_internal.h"
#include "UIAnimPannel.h"
#include "UIAnimDetail.h"
#include "UIAnimPreview.h"
#include "AnimExtern.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CEngine.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CPathMgr.h>
#include "commdlg.h"
#include <Engine/CAnimator2D.h>
#include <Engine/CAnim.h>

UIAnimPannel::UIAnimPannel()
	: UI("Animation Tool", "##Animation Tool_Main")
	, m_Atlas(nullptr)
	, m_vGridNum(ImVec2(1, 1))
	, m_vecGridSelect({ { false } })
	, m_vecAnim{}
{
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigWindowsMoveFromTitleBarOnly = true;
	Deactivate();
}

UIAnimPannel::~UIAnimPannel()
{
}

void UIAnimPannel::begin()
{
	m_DetailPannel = (UIAnimDetail*)CImGuiMgr::GetInst()->FindUI("##Animation Tool_Detail");
	m_PreviewPannel = (UIAnimPreview*)CImGuiMgr::GetInst()->FindUI("##Animation Tool_Preview");
}

void UIAnimPannel::tick()
{
	// draw grid
	ImVec2 NewGrid = m_DetailPannel->GetGridNum();
	
	if (NewGrid != m_vGridNum)
	{
		m_vGridNum = NewGrid;
		m_vGridSize = ImVec2(m_vAtlasRenderSize.x / m_vGridNum.x, m_vAtlasRenderSize.y / m_vGridNum.y);
		ResetSelectVec();
	}

	// detect close
	if (m_bPrevActive && not IsActivate())
	{
		Deactivate();
		Clear();
	}
	m_bPrevActive = IsActivate();
}

void UIAnimPannel::render_update()
{
	// menu button
	if (ImGui::Button("Compile", ImVec2(80.f, 30.f)))
		Compile();
	ImGui::SameLine();
	if (ImGui::Button("Save", ImVec2(80.f, 30.f)))
		Save();
	ImGui::SameLine();
	if (ImGui::Button("Brouse", ImVec2(100.f, 30.f)))
		OpenFileWindow();

	// UI button
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.25f, 0.25f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.25f, 0.25f, 0.25f, 1.f));

	ImGui::PushID(0);
	ImGui::SameLine(ImGui::GetWindowWidth() - 120);
	if (ImGui::Button("all", ImVec2(50.f, 30.f)))
		ResetSelectVec(true);
	ImGui::PopID();
	
	ImGui::PushID(1);
	ImGui::SameLine(ImGui::GetWindowWidth() - 60);
	if( ImGui::Button("clear", ImVec2(50.f, 30.f)))
		ResetSelectVec(false);
	
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	// atlas
	if (m_Atlas.Get())
	{
		ImGui::Text("size : %d x %d", m_Atlas.Get()->GetWidth(), m_Atlas.Get()->GetHeight());

		// draw atlas
		ImGui::BeginChild("atlas_image");
		ImVec2 vAtlasPos = ImGui::GetWindowPos();
		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImGui::Image((void*)m_Atlas.Get()->GetSRV().Get(), m_vAtlasRenderSize);

		// draw grid
		DrawGrid();

		// select grid
		ImGuiIO& io = ImGui::GetIO();
		float mouse_local_x = io.MousePos.x - pos.x;
		float mouse_local_y = io.MousePos.y - pos.y;
		UINT col = unsigned int(mouse_local_x / m_vGridSize.x);
		UINT row = unsigned int(mouse_local_y / m_vGridSize.y);
		if ((col < m_vGridNum.x && row < m_vGridNum.y) && (ImGui::IsMouseDragging(ImGuiMouseButton_Left) || ImGui::IsMouseDragging(ImGuiMouseButton_Middle)))
		{
			if ((mouse_local_x >= 0.0f) && (mouse_local_x <= m_vAtlasRenderSize.x)
				&& (mouse_local_y >= 0.0f) && (mouse_local_y <= m_vAtlasRenderSize.y))
			{
				if (mouse_local_x < 0.0f) { mouse_local_x = 0.0f; }
				else if (mouse_local_x > m_vAtlasRenderSize.x - m_vGridSize.x) { mouse_local_x = m_vAtlasRenderSize.x - m_vGridSize.x; }
				if (mouse_local_y < 0.0f) { mouse_local_y = 0.0f; }
				else if (mouse_local_y > m_vAtlasRenderSize.y - m_vGridSize.y) { mouse_local_y = m_vAtlasRenderSize.y - m_vGridSize.y; }

				if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
					m_vecGridSelect[row][col] = true;
				else
					m_vecGridSelect[row][col] = false;
			}
		}

		// draw selected
		for (int i = 0; i < m_vecGridSelect.size(); ++i)
		{
			for (int j = 0; j < m_vecGridSelect[i].size(); ++j)
			{
				if (m_vecGridSelect[i][j])
				{
					ImVec2 LeftTop = vAtlasPos + ImVec2(j * m_vGridSize.x, i * m_vGridSize.y);
					ImDrawList* draw_list = ImGui::GetForegroundDrawList();
					draw_list->AddRect(LeftTop, LeftTop + m_vGridSize, ImGui::GetColorU32(IM_COL32(255, 255, 0, 255)));			// Border
					draw_list->AddRectFilled(LeftTop, LeftTop + m_vGridSize, ImGui::GetColorU32(IM_COL32(255, 255, 0, 50)));    // Background
				}
			}
		}

		ImGui::EndChild();

		// draw mouse zoom
		if (ImGui::BeginItemTooltip())
		{
			float region_sz = 32.0f;
			mouse_local_x -= region_sz * 0.5f;
			mouse_local_y -= region_sz * 0.5f;
			float zoom = 4.0f;
			if (mouse_local_x < 0.0f) { mouse_local_x = 0.0f; }
			else if (mouse_local_x > vAtlasPos.x + m_vAtlasRenderSize.x - region_sz) { mouse_local_x = vAtlasPos.x + m_vAtlasRenderSize.x - region_sz; }
			if (mouse_local_y < 0.0f) { mouse_local_y = 0.0f; }
			else if (mouse_local_y > vAtlasPos.y + m_vAtlasRenderSize.y - region_sz) { mouse_local_y = vAtlasPos.y + m_vAtlasRenderSize.y - region_sz; }
			ImVec2 uv0 = ImVec2((mouse_local_x) / m_vAtlasRenderSize.x, (mouse_local_y) / m_vAtlasRenderSize.y);
			ImVec2 uv1 = ImVec2((mouse_local_x + region_sz) / m_vAtlasRenderSize.x, (mouse_local_y + region_sz) / m_vAtlasRenderSize.y);
			ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
			ImGui::Image(m_Atlas->GetSRV().Get(), ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), border_col);
			ImGui::EndTooltip();
		}
	}
	else
		ImGui::Text("size :");
}

void UIAnimPannel::Activate()
{
	UI::Activate();

	if (m_DetailPannel)
		m_DetailPannel->Activate();
	if (m_PreviewPannel)
		m_PreviewPannel->Activate();
}

void UIAnimPannel::Deactivate()
{
	UI::Deactivate();

	if (m_DetailPannel)
		m_DetailPannel->Deactivate();
	if (m_PreviewPannel)
		m_PreviewPannel->Deactivate();
}

void UIAnimPannel::OpenFileWindow()
{
	// 탐색창
	OPENFILENAME OFN;
	TCHAR lpstrFile[100] = L"";
	static TCHAR filter[] = L"모든 파일\0*.*\0텍스트 파일\0*.txt\0fbx 파일\0*.fbx";

	HWND hWnd = CEngine::GetInst()->GetMainWind();
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = hWnd;
	OFN.lpstrFilter = filter;
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = 100;
	wstring strInitPath = CPathMgr::GetContentPath();
	OFN.lpstrInitialDir = strInitPath.c_str();
	OFN.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// 파일 선택
	if (GetOpenFileName(&OFN))
	{
		// 현재 정보 날림
		Clear();

		// 파일이름 분석
		wchar_t szFileName[128] = {};
		wchar_t szExt[16] = {};
		_wsplitpath_s(OFN.lpstrFile, nullptr, 0, nullptr, 0, szFileName, 128, szExt, 16);
		wstring relativePath = CPathMgr::GetRelativePath(OFN.lpstrFile);
		wstring strExt = (wstring)szExt;

		if (strExt == L".PNG" || strExt == L".png" || strExt == L".JPG" || strExt == L".jpg" || strExt == L".BMP" || strExt == L".bmp")
		{
			m_AtlasKey = relativePath;
			LoadAtlas(m_AtlasKey);
		}
		else if (strExt == L".anim")
		{
			// 애니메이션 로드
			//LoadAnim(m_AtlasKey);
			// open save file
			FILE* pFile = nullptr;
			_wfopen_s(&pFile, (CPathMgr::GetContentPath() + relativePath).c_str(), L"rb");

			// save
			CAnim* pAnim = new CAnim;
			pAnim->LoadFromFile(pFile);

			// close save file
			fclose(pFile);

			// atlas 세팅
			LoadAtlas(pAnim->m_AtlasTex->GetKey());

			// Animation Frm 기록
			m_DetailPannel->SetAtlas(m_Atlas);
			m_DetailPannel->SetFrm(pAnim->m_vecFrm);

			// Play preview
			m_PreviewPannel->SetAtlas(m_Atlas);
			m_PreviewPannel->SetFrmSize(pAnim->m_vecFrm.size());
			m_PreviewPannel->Init(m_DetailPannel->GetFrmByIdx(0));

			// close save file
			fclose(pFile);
			delete pAnim;
		}
	}
}

void UIAnimPannel::Compile()
{
	if (not m_Atlas.Get()) return;

	m_vecAnim.clear();

	for (int i = 0; i < m_vecGridSelect.size(); ++i)
	{
		for (int j = 0; j < m_vecGridSelect[i].size(); ++j)
		{
			if (m_vecGridSelect[i][j])
			{
				Vec2 actualSize = Vec2(m_Atlas->GetWidth() / (float)m_vGridNum.x, m_Atlas->GetHeight() / (float)m_vGridNum.y);
				Vec2 LeftTop = Vec2(j * actualSize.x, i * actualSize.y);
				CreateFAnim(LeftTop, actualSize);
			}
		}
	}

	if (m_vecAnim.empty()) return;

	// Animation Frm 기록
	m_DetailPannel->SetAtlas(m_Atlas);
	m_DetailPannel->UpdateFrm(m_vecAnim);

	// Play preview
	m_PreviewPannel->Init(m_DetailPannel->GetFrmByIdx(0));
	m_PreviewPannel->SetAtlas(m_Atlas);
	m_PreviewPannel->SetFrmSize(m_vecAnim.size());
}

void UIAnimPannel::Save()
{
	// create anim
	CAnim* pAnim = new CAnim;
	pAnim->Create(nullptr, m_Atlas, m_DetailPannel->GetFrms(), true);
	pAnim->SetName(StrToWstr(m_DetailPannel->GetName()));

	// open save file
	FILE* pFile = nullptr;
	wstring strLevelPath = CPathMgr::GetContentPath() + (wstring)L"animation\\" + pAnim->GetName() + L".anim";
	_wfopen_s(&pFile, strLevelPath.c_str(), L"wb");

	// save
	pAnim->SaveToFile(pFile);

	// close save file
	fclose(pFile);
	delete pAnim;
}

void UIAnimPannel::LoadAtlas(const wstring& _RelativePath)
{
	m_Atlas = CAssetMgr::GetInst()->FindAsset<CTexture>(_RelativePath);
	if (not m_Atlas.Get())
		m_Atlas = CAssetMgr::GetInst()->Load<CTexture>(_RelativePath);

	m_vAtlasRenderSize = ImVec2(m_Atlas.Get()->GetWidth(), m_Atlas.Get()->GetHeight());
	float resizeScale = 1;
	float maxScale = ImGui::GetWindowHeight();
	if (m_Atlas.Get()->GetHeight() > maxScale)
	{
		resizeScale = maxScale / m_vAtlasRenderSize.y;
		m_vAtlasRenderSize.y *= resizeScale;
		m_vAtlasRenderSize.x *= resizeScale;
	}
	if (m_Atlas.Get()->GetWidth() > maxScale)
	{
		resizeScale = maxScale / m_vAtlasRenderSize.x;
		m_vAtlasRenderSize.y *= resizeScale;
		m_vAtlasRenderSize.x *= resizeScale;
	}

	ResetSelectVec();
}

bool UIAnimPannel::SelectionRect(ImVec2* start_pos, ImVec2* end_pos, ImGuiMouseButton mouse_button)
{
	IM_ASSERT(start_pos != NULL);
	IM_ASSERT(end_pos != NULL);
	if (ImGui::IsMouseClicked(mouse_button))
		*start_pos = ImGui::GetMousePos();
	if (ImGui::IsMouseDown(mouse_button))
	{
		*end_pos = ImGui::GetMousePos();
		ImDrawList* draw_list = ImGui::GetForegroundDrawList(); //ImGui::GetWindowDrawList();

		draw_list->AddRect(*start_pos, *end_pos, ImGui::GetColorU32(IM_COL32(0, 130, 216, 255)));			// Border
		draw_list->AddRectFilled(*start_pos, *end_pos, ImGui::GetColorU32(IM_COL32(0, 130, 216, 50)));		// Background
	}
	return ImGui::IsMouseReleased(mouse_button);
}

void UIAnimPannel::DrawGrid()
{
	for (int row = 0; row < m_vGridNum.y; ++row)
	{
		ImVec2 LeftTop = ImVec2(0.f, m_vGridSize.y * row);
		ImVec2 RightBot = ImVec2(0.f, m_vGridSize.y * (row + 1));

		for (int col = 0; col < m_vGridNum.x; ++col)
		{
			ImDrawList* draw_list = ImGui::GetForegroundDrawList();
			LeftTop.x = m_vGridSize.x * col;
			RightBot.x = m_vGridSize.x * (col + 1);

			draw_list->AddRect(LeftTop + ImGui::GetWindowPos(), RightBot + ImGui::GetWindowPos(), ImGui::GetColorU32(IM_COL32(255, 255, 255, 80)));   // Border
		}
	}
}

void UIAnimPannel::CreateFAnim(Vec2 _vLeftTop, Vec2 _vBackground)
{
	FAnimFrm anim = {};
	anim.fDuration = 1.f / (float)m_DetailPannel->GetFPS();
	anim.vBackgroundSize = _vBackground;
	anim.vCutSizeUV = _vBackground;
	anim.vLeftTopUV = _vLeftTop;
	anim.vOffsetUV = Vec2();

	m_vecAnim.push_back(anim);
}

void UIAnimPannel::ResetSelectVec(bool _bool)
{
	m_vecGridSelect.clear();
	m_vecGridSelect.resize(m_vGridNum.y, vector<bool>(m_vGridNum.x, _bool));
}

void UIAnimPannel::Clear()
{
	ResetSelectVec();
	m_vecAnim.clear();
	m_DetailPannel->Clear();
	m_PreviewPannel->Clear();
}
