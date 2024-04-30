#pragma once
#include <Engine/CState.h>

class CMonsterDie :
    public CState
{
private:
    Ptr<CPrefab> m_EffectParticle;
    Ptr<CPrefab> m_EffectAnim;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CMonsterDie();
    ~CMonsterDie();
};