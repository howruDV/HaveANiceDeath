#pragma once
#include <Engine/CState.h>
#include <Scripts\CHitboxScript.h>

class CScytheDissAerial :
    public CState
{
private:
    CGameObject* m_pHitbox;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CScytheDissAerial();
    ~CScytheDissAerial();
};