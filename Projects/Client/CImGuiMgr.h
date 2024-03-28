#pragma once
#include <Engine/singleton.h>
#include <Engine/CTexture.h>
#include <Engine/CPrefab.h>
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
    vector<string>              m_LayerName;
    Ptr<CTexture>               m_RTCopyTex;
    Ptr<CPrefab>                m_Prefab;
    ImVec2                      m_ViewportStart;
    ImVec2                      m_ViewportSize;
    HANDLE                      m_hNotify;

private:
    void tick();
    void render();
    void render_copytex();

    void CreateUI();
    void observe_content();
    void LoadLayerName();

public:
    void DragPrefab(DWORD_PTR _pref);
    void init(HWND _hMainWnd, ComPtr<ID3D11Device> _Device, ComPtr<ID3D11DeviceContext> _Context);
    void begin();
    void progress();

public:
    const vector<string>& GetLayerName() { return m_LayerName; }
    ImVec2 GetViewportStart() { return m_ViewportStart; }
    ImVec2 GetViewportSize() { return m_ViewportSize; }
    Vec2 GetMouseWorldPos(ImVec2 _MousePos);

public:
    UI* FindUI(const string& _strKey);
    void AddUI(const string& _strKey, UI* _UI);
};