#pragma once
#include <Engine/CState.h>

class CMonsterHit :
    public CState
{
private:
    float m_fHitBlinkTime;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CMonsterHit();
    ~CMonsterHit();
};