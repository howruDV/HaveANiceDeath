#pragma once
#include <Engine/CState.h>

class CPlayerPowerUp :
    public CState
{
private:
    Vec3 m_LandPos;
    Vec3 m_MoveTop;
    Vec3 m_TargetPos;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CPlayerPowerUp();
    ~CPlayerPowerUp();
};