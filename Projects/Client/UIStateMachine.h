#pragma once
#include "UIComponent.h"

class UIStateMachine :
    public UIComponent
{
private:
    string          m_StateName;
    string          m_AddStateKey;
    wstring         m_CurStateKey;

public:
    void StateSelect(DWORD_PTR _ptr);

private:

public:
    virtual void render_update() override;
public:
    UIStateMachine();
    ~UIStateMachine();
};

