#include "pch.h"
#include "CImGuiMgr.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "ObjectController.h"
#include "UIInspectorPannel.h"
#include "UIContentPannel.h"
#include "UIOutlinerPannel.h"
#include "UIListPannel.h"
#include "UIMenuPannel.h"
#include "UIParam.h"
#include "UIAnimPannel.h"
#include "UIAnimDetail.h"
#include "UIAnimPreview.h"
#include "UIGamePlayPannel.h"
#include "MtrlEditorUI.h"

#include <Engine/CDevice.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CTaskMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CPathMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CTransform.h>

CImGuiMgr::CImGuiMgr()
	: m_hNotify(nullptr)
{

}

CImGuiMgr::~CImGuiMgr()
{
	// ImGui Clear
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// UI Clear
	// - imGUI 해제: 최상위 부모들만 초기화
	// - UI가 가진 Childs를 직접 해제시켜줘야 함
	Delete_Map(m_mapUI);

	// 디렉터리 변경 감시 종료
	FindCloseChangeNotification(m_hNotify);
}

void CImGuiMgr::begin()
{
	for (const auto& pair : m_mapUI)
	{
		pair.second->begin();
	}
}

void CImGuiMgr::tick()
{	
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	if (CTaskMgr::GetInst()->GetChangeLevel())
	{
		LoadLayerName();
	}
	ImGui::ShowDemoWindow();
	UIParam::ResetID();

	for (const auto& pair : m_mapUI)
		pair.second->tick();
}

void CImGuiMgr::render()
{
	render_copytex();

	// UI
	for (const auto& pair : m_mapUI)
		pair.second->render();

	// Rendering
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void CImGuiMgr::render_copytex()
{
	ImGui::Begin("Viewport##GameWindow");

	Vec2 RenderResolution = CDevice::GetInst()->GetRenderResolution();
	ImVec2 RenderResol = { RenderResolution.x,RenderResolution.y };
	Ptr<CTexture> pCopyTex = CRenderMgr::GetInst()->GetRTCopyTex();

	// 현재 크기
	ImVec2 contentSize = ImGui::GetContentRegionAvail();
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
	ImVec2 Resolution = { vResolution.x,vResolution.y };
	ImVec2 LeftTopUv;
	ImVec2 RightBottom;

	LeftTopUv.x = ((vResolution.x - contentSize.x) / 2.f) / Resolution.x;
	LeftTopUv.y = ((vResolution.y - contentSize.y) / 2.f) / Resolution.y;
	RightBottom.x = 1.f - LeftTopUv.x;
	RightBottom.y = 1.f - LeftTopUv.y;

	// Image 위치 기록
	ImVec2 windowPos = ImGui::GetCursorScreenPos();
	ImVec2 windowSize = ImVec2(Resolution.x * (RightBottom.x - LeftTopUv.x), Resolution.y * (RightBottom.y - LeftTopUv.y));
	UIInspectorPannel* pInspector = (UIInspectorPannel*)CImGuiMgr::GetInst()->FindUI("##Inspector");
	m_ViewportStart = windowPos;
	m_ViewportSize = windowSize;

	// draw image
	ImGui::Image(pCopyTex->GetSRV().Get(), ImVec2(Resolution.x * (RightBottom.x - LeftTopUv.x), Resolution.y * (RightBottom.y - LeftTopUv.y)), LeftTopUv, RightBottom);

	// case: drop
	if (ImGui::IsMouseReleased(0) && ImGui::BeginDragDropTarget())
	{
		if (m_Prefab.Get())
		{
			CGameObject* pObj = m_Prefab->Instantiate();
			ImVec2 mousePosition = ImGui::GetMousePos();
			Vec2 MouseWorldPos = GetMouseWorldPos(mousePosition);
			pObj->Transform()->SetRelativePos(Vec3(MouseWorldPos.x, MouseWorldPos.y, pObj->Transform()->GetRelativePos().z));
			GamePlayStatic::SpawnGameObject(pObj, 0);
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::End();
}

void CImGuiMgr::CreateUI()
{
	UI* pUI = nullptr;

	// Inspector
	pUI = new UIInspectorPannel;
	AddUI(pUI->GetID(), pUI);

	// Content
	pUI = new UIContentPannel;
	AddUI(pUI->GetID(), pUI);

	// Outliner
	pUI = new UIOutlinerPannel;
	AddUI(pUI->GetID(), pUI);

	// Menu
	pUI = new UIMenuPannel;
	AddUI(pUI->GetID(), pUI);

	// GamePlay
	pUI = new UIGamePlayPannel;
	AddUI(pUI->GetID(), pUI);

	// List
	pUI = new UIListPannel;
	AddUI(pUI->GetID(), pUI);

	// Animation Tool
	pUI = new UIAnimPannel;
	AddUI(pUI->GetID(), pUI);
	pUI = new UIAnimDetail;
	AddUI(pUI->GetID(), pUI);
	pUI = new UIAnimPreview;
	AddUI(pUI->GetID(), pUI);

	// Material Inspector
	pUI = new UIMatEditor;
	AddUI(pUI->GetID(), pUI);
}

void CImGuiMgr::DragPrefab(DWORD_PTR _pref)
{
	Ptr<CAsset> pAsset = (CAsset*)_pref;

	if (pAsset.Get() && pAsset->GetType() == ASSET_TYPE::PREFAB)
	{
		m_Prefab = (CPrefab*)pAsset.Get();
	}
}

void CImGuiMgr::init(HWND _hMainWnd, ComPtr<ID3D11Device> _Device, ComPtr<ID3D11DeviceContext> _Context)
{
	// ---------------------------
	// ImGUI Initializing
	// ---------------------------
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;
	//io.ConfigViewportsNoDefaultParent = true;
	//io.ConfigDockingAlwaysTabBar = true;
	//io.ConfigDockingTransparentPayload = true;
	//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
	//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = 2;
	style.WindowPadding = ImVec2(10, 10);
	style.FramePadding = ImVec2(5, 3);
	style.ItemSpacing = ImVec2(7, 5);
	style.ItemInnerSpacing = ImVec2(7, 5);
	style.ScrollbarSize = 15;
	style.GrabMinSize = 13;
	style.WindowRounding = 5.3f;
	style.FrameRounding = 2;
	style.ScrollbarRounding = 12;
	style.GrabRounding = 1;
	style.TabRounding = 3;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_None;
	style.SelectableTextAlign = ImVec2(0.05f, 0.f);
	style.SeparatorTextAlign = ImVec2(0.5f, 0.7f);
	style.SeparatorTextPadding = ImVec2(20, 5);
	style.DockingSeparatorSize = 7;
	style.Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
	style.Colors[ImGuiCol_TextDisabled] = ImColor(100, 100, 100, 255);
	style.Colors[ImGuiCol_WindowBg] = ImColor(38, 38, 38, 255);
	style.Colors[ImGuiCol_ChildBg] = ImColor(38, 38, 38, 0);
	style.Colors[ImGuiCol_PopupBg] = ImColor(24, 24, 24, 230);
	style.Colors[ImGuiCol_Border] = ImColor(24, 24, 24, 128);
	style.Colors[ImGuiCol_FrameBg] = ImColor(20, 20, 20, 255);
	style.Colors[ImGuiCol_FrameBgHovered] = ImColor(10, 10, 10, 255);
	style.Colors[ImGuiCol_FrameBgActive] = ImColor(50, 50, 50, 255);
	style.Colors[ImGuiCol_TitleBg] = ImColor(24, 24, 24, 255);
	style.Colors[ImGuiCol_TitleBgActive] = ImColor(24, 24, 24, 255);
	style.Colors[ImGuiCol_MenuBarBg] = ImColor(24, 24, 24, 255);
	style.Colors[ImGuiCol_ScrollbarBg] = ImColor(24, 24, 24, 0);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImColor(140, 140, 140, 130);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(140, 140, 140, 160);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(140, 140, 140, 255);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
	style.Colors[ImGuiCol_SliderGrab] = ImColor(90, 90, 90, 255);
	style.Colors[ImGuiCol_SliderGrabActive] = ImColor(179, 179, 179, 214);
	style.Colors[ImGuiCol_Button] = ImColor(20, 20, 20, 255);
	style.Colors[ImGuiCol_ButtonHovered] = ImColor(64, 64, 64, 255);
	style.Colors[ImGuiCol_ButtonActive] = ImColor(102, 102, 102, 255);
	style.Colors[ImGuiCol_Header] = ImColor(60, 60, 60, 255);
	style.Colors[ImGuiCol_HeaderHovered] = ImColor(102, 102, 102, 255);
	style.Colors[ImGuiCol_HeaderActive] = ImColor(102, 102, 102, 255);
	style.Colors[ImGuiCol_Separator] = ImColor(110, 110, 110, 110);
	style.Colors[ImGuiCol_SeparatorHovered] = ImColor(110, 110, 110, 110);
	style.Colors[ImGuiCol_SeparatorActive] = ImColor(110, 110, 110, 255);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	style.Colors[ImGuiCol_Tab] = ImColor(60, 60, 60, 255);
	style.Colors[ImGuiCol_TabHovered] = ImColor(102, 102, 102, 255);
	style.Colors[ImGuiCol_TabActive] = ImColor(102, 102, 102, 255);
	style.Colors[ImGuiCol_TabUnfocused] = ImColor(60, 60, 60, 255);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImColor(102, 102, 102, 255);
	style.Colors[ImGuiCol_DockingPreview] = ImColor(230, 170, 0, 100);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImColor(230, 170, 0, 255);
	style.Colors[ImGuiCol_PlotHistogram] = ImColor(230, 170, 0, 255);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImColor(230, 170, 0, 255);
	style.Colors[ImGuiCol_TextSelectedBg] = ImColor(230, 170, 0, 89);
	style.Colors[ImGuiCol_DragDropTarget] = ImColor(230, 170, 0, 230);
	style.Colors[ImGuiCol_NavHighlight] = ImColor(230, 170, 0, 255);

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(_hMainWnd);
	ImGui_ImplDX11_Init(_Device.Get(), _Context.Get());

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != nullptr);

	wstring strContentPath = CPathMgr::GetContentPath();
	wstring fontPath = strContentPath + L"font\\NanumSquare_acR.ttf";
	io.Fonts->AddFontFromFileTTF(WstrToStr(fontPath).c_str(), 15.0f, NULL, io.Fonts->GetGlyphRangesKorean());

	// ---------------------------
	// Mgr Initializing
	// ---------------------------
	CreateUI();
	// Content 폴더 감시
	m_hNotify = FindFirstChangeNotification(strContentPath.c_str(), true
		, FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME
		| FILE_ACTION_ADDED | FILE_ACTION_REMOVED);
}

void CImGuiMgr::progress()
{
	tick();
	render();
	observe_content();
}

UI* CImGuiMgr::FindUI(const string& _strKey)
{
	unordered_map<string, UI*>::iterator iter = m_mapUI.find(_strKey);
	if (iter == m_mapUI.end())
		return nullptr;
	return iter->second;
}

void CImGuiMgr::AddUI(const string& _strKey, UI* _UI)
{
	UI* pUI = FindUI(_strKey);
	assert(!pUI);
	m_mapUI.insert({ _strKey, _UI });
}

void CImGuiMgr::observe_content()
{
	// WaitForSingleObject 를 이용해서 알림이 있는지 확인,
	// 대기시간은 0로 설정해서 알림이 있던 없던 바로 반환
	if (WAIT_OBJECT_0 == WaitForSingleObject(m_hNotify, 0))
	{
		// 다시 알림 활성화
		FindNextChangeNotification(m_hNotify);

		// ContentUI 에 Reload 작업 수행
		UIContentPannel* pContentUI = (UIContentPannel*)FindUI("##Content");
		pContentUI->ReloadContent();
	}
}

void CImGuiMgr::LoadLayerName()
{
	m_LayerName.clear();

	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	for (int i = 0; i < LAYER_MAX; ++i)
	{
		wstring layerName = pLevel->GetLayer(i)->GetName();
		string strLayerName = string(layerName.begin(), layerName.end());
		if (strLayerName == "")
		{
			strLayerName = std::to_string(i);
		}

		m_LayerName.push_back("[" + std::to_string(i) + "]" + " " + strLayerName);
	}
}

//int LayerIdx = -1;
//
//size_t start_pos = m_CurLayer.find("[") + 1;
//size_t end_pos = m_CurLayer.find("]");
//if (start_pos != std::string::npos && end_pos != std::string::npos)
//{
//	std::string num_str = m_CurLayer.substr(start_pos, end_pos - start_pos);
//	LayerIdx = std::stoi(num_str);
//}