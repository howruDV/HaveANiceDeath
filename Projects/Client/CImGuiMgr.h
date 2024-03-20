#pragma once
#include <Engine/singleton.h>
// =======================================
// CImGUIMgr: ImGUI process 积己 棺 包府
// =======================================

class UI;
typedef void (*CALLBACK_0)(void);
typedef void (*CALLBACK_1)(DWORD_PTR);
typedef void (*CALLBACK_2)(DWORD_PTR, DWORD_PTR);
typedef void (UI::* DELEGATE_0)();
typedef void (UI::* DELEGATE_1)(DWORD_PTR);
typedef void (UI::* DELEGATE_2)(DWORD_PTR, DWORD_PTR);

class CImGuiMgr :
    public CSingleton<CImGuiMgr>
{
    SINGLE(CImGuiMgr)

private:
    unordered_map<string, UI*>  m_mapUI;
    HANDLE                      m_hNotify;

private:
    void tick();
    void render();
    void CreateUI();
    void observe_content();

public:
    void init(HWND _hMainWnd, ComPtr<ID3D11Device> _Device, ComPtr<ID3D11DeviceContext> _Context);
    void begin();
    void progress();

public:
    UI* FindUI(const string& _strKey);
    void AddUI(const string& _strKey, UI* _UI);
};