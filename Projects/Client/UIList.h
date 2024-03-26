#pragma once
#include "UI.h"


class UIList :
    public UI
{
private:
    string              m_Filter;

    vector<string>      m_vecStr;
    string              m_strDBClicked;

    CALLBACK_1          m_CallBackFunc;

    UI*                 m_pUI;
    DELEGATE_1          m_Func;
    

public:
    void AddString(const string& _str) { m_vecStr.push_back(_str); }
    void AddString(const vector<string>& _vecStr)
    {
        m_vecStr.insert(m_vecStr.end(), _vecStr.begin(), _vecStr.end());
    }

    void SetDbClickCallBack(CALLBACK_1 _CallBack) { m_CallBackFunc = _CallBack; }
    void SetDbClickDelegate(UI* _Inst, DELEGATE_1 _MemFunc)
    {
        m_pUI = _Inst;
        m_Func = _MemFunc;
    }

    string GetDoubleClicked() { return m_strDBClicked; }



public:
    virtual void render_update() override;
    virtual void Deactivate() override;



public:
    UIList();
    ~UIList();
};

