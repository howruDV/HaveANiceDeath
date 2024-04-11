#pragma once
#include <Engine/CState.h>

class CScytheDissCrush :
    public CState
{
private:
    Vec3 m_StartPos;
    CGameObject* m_pHitbox;
    bool m_bDownFirst;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CScytheDissCrush();
    ~CScytheDissCrush();
};