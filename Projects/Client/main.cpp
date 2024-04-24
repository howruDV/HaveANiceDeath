#include "pch.h"
#include "framework.h"
#include "Client.h"
#include <crtdbg.h> // memory lick check

#include <dwmapi.h>
#include <Engine\global.h>
#include <Engine\CEngine.h>
#include <Engine/CDevice.h>
#include <Engine/CPrefab.h>

#ifdef _DEBUG
#pragma comment(lib, "Engine\\Engine_debug.lib")
#pragma comment(lib, "Scripts\\Scripts_debug.lib")
#pragma comment(lib, "States\\States_d.lib")

#include "CImGuiMgr.h"
#include "CEditorObjMgr.h"
#include "CCreateTempLevel.h"
#else
#pragma comment(lib, "Engine\\Engine.lib")
#pragma comment(lib, "Scripts\\Scripts.lib")
#pragma comment(lib, "States\\States.lib")
#endif

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "CLevelSaveLoad.h"
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CCollisionMgr.h>

//#define _RELEASE_GAME
#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;
HWND hWnd;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM); 
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    // memory lick check
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetBreakAlloc(136);

    MyRegisterClass(hInstance);

    // ------------------
    // Init
    // ------------------
    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));
    MSG msg;

#ifdef _DEBUG
    RECT rect = { 0,0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
    SetWindowPos(hWnd, nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top - 79, 0);
    GetClientRect(hWnd, &rect);

    // window size, position
    Vec2 WinSize = Vec2((float)rect.right, (float)rect.bottom);
    rect = { 0, 0, (int)WinSize.x, (int)WinSize.y };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
    SetWindowPos(hWnd, nullptr, -10, 0, rect.right - rect.left, rect.bottom - rect.top, 0);
#else
    Vec2 WinSize = Vec2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
    LONG_PTR style = GetWindowLongPtr(hWnd, GWL_STYLE);
    style &= ~WS_OVERLAPPEDWINDOW;
    style |= WS_POPUP;
    SetWindowLongPtr(hWnd, GWL_STYLE, style);
    SetWindowPos(hWnd, HWND_TOP, 0, 0, WinSize.x, WinSize.y, SWP_FRAMECHANGED | SWP_SHOWWINDOW);

    // window size, position
    //Vec2 WinSize = Vec2 (1600,900);
    //RECT rect = { 0, 0, (int)WinSize.x, (int)WinSize.y };
    //AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
    //SetWindowPos(hWnd, nullptr, -10, 0, rect.right - rect.left, rect.bottom - rect.top, 0);
#endif

    // window style
    COLORREF DARK_COLOR = 0x00151515;
    BOOL SET_BORDER_COLOR = SUCCEEDED(DwmSetWindowAttribute(hWnd, DWMWINDOWATTRIBUTE::DWMWA_BORDER_COLOR,&DARK_COLOR, sizeof(DARK_COLOR)));
    BOOL SET_CAPTION_COLOR = SUCCEEDED(DwmSetWindowAttribute(hWnd, DWMWINDOWATTRIBUTE::DWMWA_CAPTION_COLOR,&DARK_COLOR, sizeof(DARK_COLOR)));

    // CEnigne init
    if (FAILED(CEngine::GetInst()->init(hWnd, WinSize)))
    {
        MessageBox(nullptr, L"Failed to initialize CEngine", L"Faile to initialize", MB_OK);
        return 0;
    }

    CPrefab::GAMEOBJECT_SAVE = &CLevelSaveLoad::SaveGameObject;
    CPrefab::GAMEOBJECT_LOAD = &CLevelSaveLoad::LoadGameObject;
    CLevel::LEVEL_SAVE = &CLevelSaveLoad::SaveLevel;
    CLevel::LEVEL_LOAD = &CLevelSaveLoad::LoadLevel;

#ifndef _RELEASE_GAME
    // create temp level
    CCreateTempLevel::CreateTempLevel();

    // EditorObjectManager 초기화
    CEditorObjMgr::GetInst()->init();

    // ImGUI init
    CImGuiMgr::GetInst()->init(hWnd, DEVICE, CONTEXT);
    CImGuiMgr::GetInst()->begin();
#else
    CLevel* pLevel = CLevelSaveLoad::LoadLevel(L"level\\Field1");
    CLevelMgr::GetInst()->ChangeLevel(pLevel, LEVEL_STATE::PLAY);
    CCollisionMgr::GetInst()->LayerCheck(3, 4);
    CCollisionMgr::GetInst()->LayerCheck(3, 6);
    CCollisionMgr::GetInst()->LayerCheck(4, 6);
#endif

    // ------------------
    // main loop
    // ------------------
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            // update
            CEngine::GetInst()->progress();

#ifndef _RELEASE_GAME
            CEditorObjMgr::GetInst()->progress();
            CImGuiMgr::GetInst()->progress();
#endif

            // Engine + ImGUI의 렌더링 최종 결과 present
            CDevice::GetInst()->Present();
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//  용도: 창 클래스를 등록합니다.
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr; //MAKEINTRESOURCEW(IDC_CLIENT);
    wcex.lpszClassName  = L"Have A Nice Death";
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//   주석:
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
   hWnd = CreateWindowW(L"Have A Nice Death", L"Have A Nice Death", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//  용도: 주 창의 메시지를 처리합니다.
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;
    
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;

            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;

            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_MOUSEWHEEL:
        {
            short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            CKeyMgr::GetInst()->SetMouseWheel(zDelta);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_DPICHANGED:
        // 해상도가 변하는 경우 (듀얼모니터 등)
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;

    default:
        {
            CKeyMgr::GetInst()->SetMouseWheel(0);
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}