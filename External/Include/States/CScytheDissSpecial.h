#pragma once
#include <Engine/CState.h>

class CScytheDissSpecial :
    public CState
{
private:
    Vec3 m_OrginScale;
    CGameObject* m_pHitbox;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CScytheDissSpecial();
    ~CScytheDissSpecial();
};