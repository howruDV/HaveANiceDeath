#include "pch.h"
#include "imgui_internal.h"
#include "UIAnimPannel.h"
#include "UIAnimDetail.h"
#include "UIAnimPreview.h"
#include <Shobjidl.h> // 파일 대화 상자 헤더
#include <iostream>

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
	if (ImGui::Button("Brouse", ImVec2(80.f, 30.f)))
		OpenFileWindow();

	ImGui::SameLine();
	if (ImGui::Button("Meta", ImVec2(80.f, 30.f)))
	{
		m_vecOffset.clear();

		// File 다중 선택
		vector<wstring> FilesName;

		// 파일 대화창 열기
		openFileDialog(FilesName);

		for (size_t i = 0; i < FilesName.size(); ++i)
		{
			Vec2 CurFrmOffset = LoadMeta(FilesName[i]);
			m_vecOffset.push_back(CurFrmOffset);
		}
	}

	// UI button
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.25f, 0.25f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.25f, 0.25f, 0.25f, 1.f));

	ImGui::SameLine();
	ImGui::PushID(2);
	if (ImGui::Button("Meta clear", ImVec2(80.f, 30.f)))
		ClearOffset();
	ImGui::PopID();

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
	wchar_t szSelect[256] = {};
	OPENFILENAME OFN = {};
	OFN.lStructSize = sizeof(OFN);
	OFN.hwndOwner = nullptr;
	OFN.lpstrFile = szSelect;
	OFN.lpstrFile[0] = '\0';
	OFN.nMaxFile = sizeof(szSelect);
	OFN.lpstrFilter = L"ALL\0*.*\0애니메이션 파일\0*.anim";
	OFN.nFilterIndex = 1;
	OFN.lpstrFileTitle = NULL;
	OFN.nMaxFileTitle = 0;
	wstring strInitPath = CPathMgr::GetContentPath() + (wstring)L"texture\\";
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
			// open save file
			FILE* pFile = nullptr;
			_wfopen_s(&pFile, (CPathMgr::GetContentPath() + relativePath).c_str(), L"rb");

			// save
			CAnim* pAnim = new CAnim;
			pAnim->LoadFromFile(pFile);

			// close save file
			//fclose(pFile);

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

	// 오프셋 저장
	if (m_vecAnim.size() != m_vecOffset.size())
	{
		MessageBoxA(nullptr, "선택된 프레임의 개수와 Offset파일의 개수가 다릅니다.", "Compile Failed", MB_OK);
		//return;
	}
	else
	{
		// 저장
		for (size_t i = 0; i < m_vecAnim.size(); ++i)
		{
			m_vecAnim[i].vOffsetUV = m_vecOffset[i];
		}
	}

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
	pAnim->SetName(StrToWstr(m_DetailPannel->GetAnimName()));

	// open save file
	FILE* pFile = nullptr;
	wstring strLevelPath = CPathMgr::GetContentPath() + (wstring)L"animation\\" + pAnim->GetName() + L".anim";
	_wfopen_s(&pFile, strLevelPath.c_str(), L"wb");

	// save
	pAnim->SaveToFile(pFile);

	// close save file
	fclose(pFile);
	delete pAnim;

	MessageBoxA(nullptr, "Animation Saved", "Animation Saved", MB_OK);
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

void UIAnimPannel::ClearOffset()
{
	for (size_t i = 0; i < m_vecAnim.size(); ++i)
	{
		m_vecAnim[i].vOffsetUV = Vec2(0.f, 0.f);
	}

	m_vecOffset.clear();
	m_DetailPannel->UpdateFrm(m_vecAnim);
}

// Usage: meta file 여는용 (최신버전)
void UIAnimPannel::openFileDialog(vector<wstring>& _FilesName)
{
	IFileOpenDialog* pFileDialog;
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileDialog));
	if (FAILED(hr)) {
		std::cerr << "Failed to create FileOpenDialog instance" << std::endl;
		return;
	}

	if (SUCCEEDED(hr)) {
		// 원하는 초기 디렉토리 경로를 여기에 설정
		wstring MetaPath = CPathMgr::GetContentPath() + (wstring)L"meta\\";
		PWSTR initialDir = (PWSTR)(MetaPath.c_str());
		IShellItem* pInitialDirItem;

		hr = SHCreateItemFromParsingName(initialDir, NULL, IID_IShellItem, reinterpret_cast<void**>(&pInitialDirItem));

		// 파일 대화 상자 인터페이스에 초기 디렉토리 설정
		hr = pFileDialog->SetFolder(pInitialDirItem);
	}

	// 다중 선택 가능 설정
	DWORD dwOptions;
	hr = pFileDialog->GetOptions(&dwOptions);
	if (SUCCEEDED(hr)) {
		hr = pFileDialog->SetOptions(dwOptions | FOS_ALLOWMULTISELECT);
	}

	// 파일 필터 설정
	COMDLG_FILTERSPEC fileTypes[] = { { L"All Files", L"*.*" }, { L"Text Files", L"*.txt" }, { L"FBX Files", L"*.fbx" } };
	hr = pFileDialog->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);
	if (FAILED(hr)) {
		std::cerr << "Failed to set file types" << std::endl;
		pFileDialog->Release();
		return;
	}

	// 대화 상자 열기
	hr = pFileDialog->Show(NULL);
	if (FAILED(hr)) {
		std::cerr << "Failed to open FileOpenDialog" << std::endl;
		pFileDialog->Release();
		return;
	}

	// 선택된 파일 목록 가져오기
	IShellItemArray* pItems;
	hr = pFileDialog->GetResults(&pItems);
	if (FAILED(hr)) {
		std::cerr << "Failed to get selected items" << std::endl;
		pFileDialog->Release();
		return;
	}

	// 선택된 파일들의 경로 가져오기
	DWORD itemCount;
	hr = pItems->GetCount(&itemCount);
	if (SUCCEEDED(hr)) {
		for (DWORD i = 0; i < itemCount; ++i) {
			IShellItem* pItem;
			hr = pItems->GetItemAt(i, &pItem);
			if (SUCCEEDED(hr)) {
				PWSTR pszFilePath;
				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
				if (SUCCEEDED(hr)) {
					_FilesName.push_back(pszFilePath);
					CoTaskMemFree(pszFilePath);
				}
				pItem->Release();
			}
		}
	}
	pItems->Release();
	pFileDialog->Release();
}

Vec2 UIAnimPannel::LoadMeta(const wstring& _strMetaRelativePath)
{
	Vec2 retVec = { -1,-1 };
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, (_strMetaRelativePath).c_str(), L"r");

	if (nullptr == pFile)
	{
		MessageBoxA(nullptr, "Meta 파일이 존재하지 않습니다.", "Meta File No Exist!", MB_OK);
		return {};
	}

	// Animation 이름 로드

	while (true)
	{
		wchar_t szRead[256] = {};
		float tmpfloat = -1.f;

		if (EOF == fwscanf_s(pFile, L"%s", szRead, 256))
		{
			break;
		}


		if (!wcscmp(szRead, L"m_Offset:"))
		{
			while (true)
			{
				fwscanf_s(pFile, L"%s", szRead, 256);

				if (!wcscmp(szRead, L"{x:"))
				{
					fwscanf_s(pFile, L"%f", &retVec.x);
				}
				if (!wcscmp(szRead, L"y:"))
				{
					fwscanf_s(pFile, L"%s", szRead, 256);

					int length = (int)wcslen(szRead);

					// 끝에 한글자 잘라야됨 1.24} 라고 되어있음
					if (length > 0) {
						szRead[length - 1] = '\0';
					}

					wchar_t* end;
					float tmp = wcstof(szRead, &end);

					if (*end == L'\0') {
						retVec.y = -tmp;
					}

					return retVec;
				}

				// 탈출 조건
				if (!wcscmp(szRead, L"m_Border:"))
				{
					break;
				}
			}

		}
		// 탈출 조건
		if (!wcscmp(szRead, L"m_Border:"))
			break;

	}

	fclose(pFile);

	return retVec;
}