#pragma once
#include <Engine/CState.h>
#include <Engine/CPrefab.h>

class CPlayerMgr;

class CPlayerDash :
    public CState
{
private:
    CPlayerMgr* m_PlayerMgr;
    Ptr<CPrefab> m_EffectDash;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CPlayerDash();
    ~CPlayerDash();
};
