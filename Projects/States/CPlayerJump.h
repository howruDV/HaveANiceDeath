#pragma once
#include <Engine/CState.h>

class CPlayerMgr;

class CPlayerJump :
    public CState
{
private:
    CPlayerMgr* m_PlayerMgr;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CPlayerJump();
    ~CPlayerJump();
};