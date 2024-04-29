#pragma once
#include <Engine/CState.h>
#include <Engine/CPrefab.h>

class CPlayerHit :
    public CState
{
private:
    Ptr<CPrefab> m_EffectHit;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CPlayerHit();
    ~CPlayerHit();
};