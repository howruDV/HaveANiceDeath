#pragma once
#include <Engine/CGameObject.h>

// =======================================
// CEditorObj: Editor �� �����ϴ� GameObject
// =======================================

class CEditorObj :
    public CGameObject
{
public:
    virtual void finaltick() override;
};

