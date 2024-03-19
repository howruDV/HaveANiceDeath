#pragma once
#include "UI.h"
#include "CImGuiMgr.h"
// =======================================
// UI: List를 그리는 Pannel
// =======================================
// - Callback, Delegate 예시

class UIListPannel :
    public UI
{
private:
    vector<string>  m_vecStr;
    string          m_strDBClicked;
    CALLBACK_1      m_CallbackFunc;
    DELEGATE_1      m_DelegateFunc;
    UI*             m_pUI;

public:
    virtual void render_update() override;
    virtual void Deactivate() override;

public:
    void AddString(const string& _str) { m_vecStr.push_back(_str); }
    void AddString(const vector<string>& _vecStr) { m_vecStr.insert(m_vecStr.end(), _vecStr.begin(), _vecStr.end()); }
    void SetDBClickCallback(CALLBACK_1 _Callback) { m_CallbackFunc = _Callback; }
    void SetDBClickeDelegate(UI* _Inst, DELEGATE_1 _Delegeate) { m_pUI = _Inst; m_DelegateFunc = _Delegeate; }
    string GetDBClicked() { m_strDBClicked; }

public:
    UIListPannel();
    ~UIListPannel();
};

