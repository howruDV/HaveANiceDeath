#pragma once
#include <Engine/CState.h>
#include <Engine/CPrefab.h>

class CPlayerPowerUp :
    public CState
{
private:
    Ptr<CPrefab> m_bEffectRepulsive;
    Vec3 m_LandPos;
    Vec3 m_MoveTop;
    Vec3 m_TargetPos;
    bool m_bSpawnEffect;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CPlayerPowerUp();
    ~CPlayerPowerUp();
};