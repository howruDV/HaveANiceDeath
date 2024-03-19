#pragma once
#include <Engine/CGameObject.h>

// =======================================
// CEditorObj: Editor 상에 존재하는 GameObject
// =======================================

class CEditorObj :
    public CGameObject
{
public:
    virtual void finaltick() override;
};

