#pragma once
#include <Engine/CState.h>

class CScytheDissUp :
    public CState
{
private:
    Vec3 m_MoveTop;
    Vec3 m_TargetPos;
    CGameObject* m_pHitbox;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CScytheDissUp();
    ~CScytheDissUp();
};