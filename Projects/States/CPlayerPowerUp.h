#pragma once
#include <Engine/CState.h>

class CPlayerPowerUp :
    public CState
{
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CPlayerPowerUp();
    ~CPlayerPowerUp();
};