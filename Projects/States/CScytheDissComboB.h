#pragma once
#include <Engine/CState.h>

class CScytheDissComboB :
    public CState
{
private:
    bool m_bCombo;
    CGameObject* m_pHitbox;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CScytheDissComboB();
    ~CScytheDissComboB();
};