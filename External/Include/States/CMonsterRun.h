#pragma once
#include <Engine/CState.h>

class CMonsterRun :
    public CState
{
public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CMonsterRun();
    ~CMonsterRun();
};