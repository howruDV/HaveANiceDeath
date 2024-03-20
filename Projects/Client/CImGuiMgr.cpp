#include "pch.h"
#include "CImGuiMgr.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "UIInspectorPannel.h"
#include "UIContentPannel.h"
#include "UIOutlinerPannel.h"
#include "UIListPannel.h"
#include "UIMenuPannel.h"
#include "UIParam.h"
#include "UIAnimPannel.h"
#include "UIAnimDetail.h"
#include "UIAnimPreview.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CGameObject.h>
#include <Engine/CPathMgr.h>

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
	ImGui::ShowDemoWindow();
	UIParam::ResetID();

	for (const auto& pair : m_mapUI)
		pair.second->tick();
}

void CImGuiMgr::render()
{
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
	style.WindowRounding = 5.3f;
	style.FrameRounding = 2.3f;
	style.ScrollbarRounding = 0;
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.25f, 0.25f, 0.25f, 1.f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);

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
	wstring fontPath = strContentPath + L"font\AppleSDGothicNeoL.ttf";
	//io.Fonts->AddFontFromFileTTF(WstrToStr(fontPath).c_str(), 17.0f, NULL, io.Fonts->GetGlyphRangesKorean());

	// ---------------------------
	// Mgr Initializing
	// ---------------------------
	CreateUI();
	// Content 폴더 감시
	m_hNotify = FindFirstChangeNotification(strContentPath.c_str(), true
		, FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME
		| FILE_ACTION_ADDED | FILE_ACTION_REMOVED);

	//CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	//CGameObject* pObject = pCurLevel->FindObjectByName(L"Player");
	//((UIInspectorPannel*)FindUI("##Inspector"))->SetTargetObject(pObject);
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