#pragma once
#include <Engine/CState.h>
#include <Engine/CPrefab.h>

class CPlayerJumpStart :
    public CState
{
private:
    Ptr<CPrefab> m_EffectJump;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CPlayerJumpStart();
    ~CPlayerJumpStart();
};