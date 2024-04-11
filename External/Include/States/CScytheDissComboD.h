#pragma once
#include <Engine/CState.h>

class CScytheDissComboD :
    public CState
{
private:
    CGameObject* m_pHitbox;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CScytheDissComboD();
    ~CScytheDissComboD();
};

