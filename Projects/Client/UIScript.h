#pragma once
#include "UIComponent.h"
#include <Engine/CScript.h>

class UIScript :
    public UIComponent
{
private:
    CScript* m_TargetScript;

public:
    void SetScript(CScript* _Script);

    virtual void render_update() override;

public:
    UIScript();
    ~UIScript();
};

