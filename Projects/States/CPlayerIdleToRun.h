#pragma once
#include <Engine/CState.h>

class CPlayerMgr;

class CPlayerIdleToRun :
    public CState
{
public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CPlayerIdleToRun();
    ~CPlayerIdleToRun();
};