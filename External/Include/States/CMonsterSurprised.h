#pragma once
#include <Engine/CState.h>
#include <Engine/CPrefab.h>

class CMonsterSurprised :
    public CState
{
private:
    Ptr<CPrefab> m_EffectSurprised;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CMonsterSurprised();
    ~CMonsterSurprised();
};